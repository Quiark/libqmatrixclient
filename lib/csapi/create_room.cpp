/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#include "create_room.h"

#include "converters.h"

#include <QtCore/QStringBuilder>

using namespace QMatrixClient;

static const auto basePath = QStringLiteral("/_matrix/client/r0");

namespace QMatrixClient
{
    // Converters

    QJsonObject toJson(const CreateRoomJob::Invite3pid& pod)
    {
        QJsonObject jo;
        addParam<>(jo, QStringLiteral("id_server"), pod.idServer);
        addParam<>(jo, QStringLiteral("medium"), pod.medium);
        addParam<>(jo, QStringLiteral("address"), pod.address);
        return jo;
    }

    QJsonObject toJson(const CreateRoomJob::StateEvent& pod)
    {
        QJsonObject jo;
        addParam<>(jo, QStringLiteral("type"), pod.type);
        addParam<IfNotEmpty>(jo, QStringLiteral("state_key"), pod.stateKey);
        addParam<>(jo, QStringLiteral("content"), pod.content);
        return jo;
    }
} // namespace QMatrixClient

class CreateRoomJob::Private
{
    public:
        QString roomId;
};

static const auto CreateRoomJobName = QStringLiteral("CreateRoomJob");

CreateRoomJob::CreateRoomJob(const QString& visibility, const QString& roomAliasName, const QString& name, const QString& topic, const QStringList& invite, const QVector<Invite3pid>& invite3pid, const QString& roomVersion, const QJsonObject& creationContent, const QVector<StateEvent>& initialState, const QString& preset, bool isDirect, const QJsonObject& powerLevelContentOverride)
    : BaseJob(HttpVerb::Post, CreateRoomJobName,
        basePath % "/createRoom")
    , d(new Private)
{
    QJsonObject _data;
    addParam<IfNotEmpty>(_data, QStringLiteral("visibility"), visibility);
    addParam<IfNotEmpty>(_data, QStringLiteral("room_alias_name"), roomAliasName);
    addParam<IfNotEmpty>(_data, QStringLiteral("name"), name);
    addParam<IfNotEmpty>(_data, QStringLiteral("topic"), topic);
    addParam<IfNotEmpty>(_data, QStringLiteral("invite"), invite);
    addParam<IfNotEmpty>(_data, QStringLiteral("invite_3pid"), invite3pid);
    addParam<IfNotEmpty>(_data, QStringLiteral("room_version"), roomVersion);
    addParam<IfNotEmpty>(_data, QStringLiteral("creation_content"), creationContent);
    addParam<IfNotEmpty>(_data, QStringLiteral("initial_state"), initialState);
    addParam<IfNotEmpty>(_data, QStringLiteral("preset"), preset);
    addParam<IfNotEmpty>(_data, QStringLiteral("is_direct"), isDirect);
    addParam<IfNotEmpty>(_data, QStringLiteral("power_level_content_override"), powerLevelContentOverride);
    setRequestData(_data);
}

CreateRoomJob::~CreateRoomJob() = default;

const QString& CreateRoomJob::roomId() const
{
    return d->roomId;
}

BaseJob::Status CreateRoomJob::parseJson(const QJsonDocument& data)
{
    auto json = data.object();
    if (!json.contains("room_id"_ls))
        return { JsonParseError,
            "The key 'room_id' not found in the response" };
    d->roomId = fromJson<QString>(json.value("room_id"_ls));
    return Success;
}

