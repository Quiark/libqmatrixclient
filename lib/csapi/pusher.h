/******************************************************************************
 * THIS FILE IS GENERATED - ANY EDITS WILL BE OVERWRITTEN
 */

#pragma once

#include "jobs/basejob.h"

#include <QtCore/QVector>
#include "converters.h"

namespace QMatrixClient
{
    // Operations

    /// Gets the current pushers for the authenticated user
    /// 
    /// Gets all currently active pushers for the authenticated user
    class GetPushersJob : public BaseJob
    {
        public:
            // Inner data structures

            /// A dictionary of information for the pusher implementation
            /// itself.
            struct PusherData
            {
        /// Required if ``kind`` is ``http``. The URL to use to send
        /// notifications to.
                QString url;
            };

            /// Gets all currently active pushers for the authenticated user
            struct Pusher
            {
        /// This is a unique identifier for this pusher. See `/set` for
        /// more detail.
        /// Max length, 512 bytes.
                QString pushkey;
        /// The kind of pusher. ``"http"`` is a pusher that
        /// sends HTTP pokes.
                QString kind;
        /// This is a reverse-DNS style identifier for the application.
        /// Max length, 64 chars.
                QString appId;
        /// A string that will allow the user to identify what application
        /// owns this pusher.
                QString appDisplayName;
        /// A string that will allow the user to identify what device owns
        /// this pusher.
                QString deviceDisplayName;
        /// This string determines which set of device specific rules this
        /// pusher executes.
                QString profileTag;
        /// The preferred language for receiving notifications (e.g. 'en'
        /// or 'en-US')
                QString lang;
        /// A dictionary of information for the pusher implementation
        /// itself.
                Omittable<PusherData> data;
            };

            // Construction/destruction

            explicit GetPushersJob();

            /*! Construct a URL without creating a full-fledged job object
             *
             * This function can be used when a URL for
             * GetPushersJob is necessary but the job
             * itself isn't.
             */
            static QUrl makeRequestUrl(QUrl baseUrl);

            ~GetPushersJob() override;

            // Result properties

            /// An array containing the current pushers for the user
            const QVector<Pusher>& pushers() const;

        protected:
            Status parseJson(const QJsonDocument& data) override;

        private:
            class Private;
            QScopedPointer<Private> d;
    };

    /// Modify a pusher for this user on the homeserver.
    /// 
    /// This endpoint allows the creation, modification and deletion of `pushers`_
    /// for this user ID. The behaviour of this endpoint varies depending on the
    /// values in the JSON body.
    class PostPusherJob : public BaseJob
    {
        public:
            // Inner data structures

            /// A dictionary of information for the pusher implementation
            /// itself. If ``kind`` is ``http``, this should contain ``url``
            /// which is the URL to use to send notifications to.
            struct PusherData
            {
        /// Required if ``kind`` is ``http``. The URL to use to send
        /// notifications to.
                QString url;
            };

            // Construction/destruction

            /*! Modify a pusher for this user on the homeserver.
             * \param pushkey 
             *   This is a unique identifier for this pusher. The value you
             *   should use for this is the routing or destination address
             *   information for the notification, for example, the APNS token
             *   for APNS or the Registration ID for GCM. If your notification
             *   client has no such concept, use any unique identifier.
             *   Max length, 512 bytes.
             * \param kind 
             *   The kind of pusher to configure. ``"http"`` makes a pusher that
             *   sends HTTP pokes. ``null`` deletes the pusher.
             * \param appId 
             *   This is a reverse-DNS style identifier for the application.
             *   It is recommended that this end with the platform, such that
             *   different platform versions get different app identifiers.
             *   Max length, 64 chars.
             * \param appDisplayName 
             *   A string that will allow the user to identify what application
             *   owns this pusher.
             * \param deviceDisplayName 
             *   A string that will allow the user to identify what device owns
             *   this pusher.
             * \param lang 
             *   The preferred language for receiving notifications (e.g. 'en'
             *   or 'en-US')
             * \param data 
             *   A dictionary of information for the pusher implementation
             *   itself. If ``kind`` is ``http``, this should contain ``url``
             *   which is the URL to use to send notifications to.
             * \param profileTag 
             *   This string determines which set of device specific rules this
             *   pusher executes.
             * \param append 
             *   If true, the homeserver should add another pusher with the
             *   given pushkey and App ID in addition to any others with
             *   different user IDs. Otherwise, the homeserver must remove any
             *   other pushers with the same App ID and pushkey for different
             *   users. The default is ``false``.
             */
            explicit PostPusherJob(const QString& pushkey, const QString& kind, const QString& appId, const QString& appDisplayName, const QString& deviceDisplayName, const QString& lang, const PusherData& data, const QString& profileTag = {}, bool append = false);
    };
} // namespace QMatrixClient
