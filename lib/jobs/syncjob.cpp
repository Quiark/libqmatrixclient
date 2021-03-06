/******************************************************************************
 * Copyright (C) 2016 Felix Rohrbach <kde@fxrh.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "syncjob.h"

#include "events/eventloader.h"

#include <QtCore/QElapsedTimer>

using namespace QMatrixClient;

static size_t jobId = 0;

SyncJob::SyncJob(const QString& since, const QString& filter, int timeout,
                 const QString& presence)
    : BaseJob(HttpVerb::Get, QStringLiteral("SyncJob-%1").arg(++jobId),
              QStringLiteral("_matrix/client/r0/sync"))
{
    setLoggingCategory(SYNCJOB);
    QUrlQuery query;
    if( !filter.isEmpty() )
        query.addQueryItem(QStringLiteral("filter"), filter);
    if( !presence.isEmpty() )
        query.addQueryItem(QStringLiteral("set_presence"), presence);
    if( timeout >= 0 )
        query.addQueryItem(QStringLiteral("timeout"), QString::number(timeout));
    if( !since.isEmpty() )
        query.addQueryItem(QStringLiteral("since"), since);
    setRequestQuery(query);

    setMaxRetries(std::numeric_limits<int>::max());
}

QString SyncData::nextBatch() const
{
    return nextBatch_;
}

SyncDataList&& SyncData::takeRoomData()
{
    return std::move(roomData);
}

Events&& SyncData::takePresenceData()
{
    return std::move(presenceData);
}

Events&& SyncData::takeAccountData()
{
    return std::move(accountData);
}

Events&&SyncData::takeToDeviceEvents()
{
    return std::move(toDeviceEvents);
}

template <typename EventsArrayT, typename StrT>
inline EventsArrayT load(const QJsonObject& batches, StrT keyName)
{
    return fromJson<EventsArrayT>(batches[keyName].toObject().value("events"_ls));
}

BaseJob::Status SyncJob::parseJson(const QJsonDocument& data)
{
    return d.parseJson(data);
}

BaseJob::Status SyncData::parseJson(const QJsonDocument &data)
{
    QElapsedTimer et; et.start();

    auto json = data.object();
    nextBatch_ = json.value("next_batch"_ls).toString();
    presenceData = load<Events>(json, "presence"_ls);
    accountData = load<Events>(json, "account_data"_ls);
    toDeviceEvents = load<Events>(json, "to_device"_ls);

    auto rooms = json.value("rooms"_ls).toObject();
    JoinStates::Int ii = 1; // ii is used to make a JoinState value
    auto totalRooms = 0;
    auto totalEvents = 0;
    for (size_t i = 0; i < JoinStateStrings.size(); ++i, ii <<= 1)
    {
        const auto rs = rooms.value(JoinStateStrings[i]).toObject();
        // We have a Qt container on the right and an STL one on the left
        roomData.reserve(static_cast<size_t>(rs.size()));
        for(auto roomIt = rs.begin(); roomIt != rs.end(); ++roomIt)
        {
            roomData.emplace_back(roomIt.key(), JoinState(ii),
                                  roomIt.value().toObject());
            const auto& r = roomData.back();
            totalEvents += r.state.size() + r.ephemeral.size() +
                           r.accountData.size() + r.timeline.size();
        }
        totalRooms += rs.size();
    }
    if (totalRooms > 9 || et.nsecsElapsed() >= profilerMinNsecs())
        qCDebug(PROFILER) << "*** SyncData::parseJson(): batch with"
                          << totalRooms << "room(s),"
                          << totalEvents << "event(s) in" << et;
    return BaseJob::Success;
}

const QString SyncRoomData::UnreadCountKey =
        QStringLiteral("x-qmatrixclient.unread_count");

SyncRoomData::SyncRoomData(const QString& roomId_, JoinState joinState_,
                           const QJsonObject& room_)
    : roomId(roomId_)
    , joinState(joinState_)
    , state(load<StateEvents>(room_,
                joinState == JoinState::Invite ? "invite_state"_ls : "state"_ls))
{
    switch (joinState) {
        case JoinState::Join:
            ephemeral = load<Events>(room_, "ephemeral"_ls);
            FALLTHROUGH;
        case JoinState::Leave:
        {
            accountData = load<Events>(room_, "account_data"_ls);
            timeline = load<RoomEvents>(room_, "timeline"_ls);
            const auto timelineJson = room_.value("timeline"_ls).toObject();
            timelineLimited = timelineJson.value("limited"_ls).toBool();
            timelinePrevBatch = timelineJson.value("prev_batch"_ls).toString();

            break;
        }
        default: /* nothing on top of state */;
    }

    const auto unreadJson = room_.value("unread_notifications"_ls).toObject();
    unreadCount = unreadJson.value(UnreadCountKey).toInt(-2);
    highlightCount = unreadJson.value("highlight_count"_ls).toInt();
    notificationCount = unreadJson.value("notification_count"_ls).toInt();
    if (highlightCount > 0 || notificationCount > 0)
        qCDebug(SYNCJOB) << "Room" << roomId_
                         << "has highlights:" << highlightCount
                         << "and notifications:" << notificationCount;
}
