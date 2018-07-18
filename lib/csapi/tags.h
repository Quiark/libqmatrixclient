/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "jobs/basejob.h"

#include <QtCore/QJsonObject>

namespace QMatrixClient
{
    // Operations

    /// List the tags for a room.
    /// 
    /// List the tags set by a user on a room.
    class GetRoomTagsJob : public BaseJob
    {
        public:
            /*! List the tags for a room.
             * \param userId 
             *   The id of the user to get tags for. The access token must be
             *   authorized to make requests for this user id.
             * \param roomId 
             *   The id of the room to get tags for.
             */
            explicit GetRoomTagsJob(const QString& userId, const QString& roomId);

            /*! Construct a URL without creating a full-fledged job object
             *
             * This function can be used when a URL for
             * GetRoomTagsJob is necessary but the job
             * itself isn't.
             */
            static QUrl makeRequestUrl(QUrl baseUrl, const QString& userId, const QString& roomId);

            ~GetRoomTagsJob() override;

            // Result properties

            /// List the tags set by a user on a room.
            const QJsonObject& tags() const;

        protected:
            Status parseJson(const QJsonDocument& data) override;

        private:
            class Private;
            QScopedPointer<Private> d;
    };

    /// Add a tag to a room.
    /// 
    /// Add a tag to the room.
    class SetRoomTagJob : public BaseJob
    {
        public:
            /*! Add a tag to a room.
             * \param userId 
             *   The id of the user to add a tag for. The access token must be
             *   authorized to make requests for this user id.
             * \param roomId 
             *   The id of the room to add a tag to.
             * \param tag 
             *   The tag to add.
             * \param body 
             *   Extra data for the tag, e.g. ordering.
             */
            explicit SetRoomTagJob(const QString& userId, const QString& roomId, const QString& tag, const QJsonObject& body = {});
    };

    /// Remove a tag from the room.
    /// 
    /// Remove a tag from the room.
    class DeleteRoomTagJob : public BaseJob
    {
        public:
            /*! Remove a tag from the room.
             * \param userId 
             *   The id of the user to remove a tag for. The access token must be
             *   authorized to make requests for this user id.
             * \param roomId 
             *   The id of the room to remove a tag from.
             * \param tag 
             *   The tag to remove.
             */
            explicit DeleteRoomTagJob(const QString& userId, const QString& roomId, const QString& tag);

            /*! Construct a URL without creating a full-fledged job object
             *
             * This function can be used when a URL for
             * DeleteRoomTagJob is necessary but the job
             * itself isn't.
             */
            static QUrl makeRequestUrl(QUrl baseUrl, const QString& userId, const QString& roomId, const QString& tag);

    };
} // namespace QMatrixClient
