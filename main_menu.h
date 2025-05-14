#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QStackedWidget>
#include <QMap>
#include <QPair>
#include <QStringList>

#include "database_manager.h"
#include "view_group_chat.h"
#include "view_pvt_chat.h"
#include "group_chat_list.h"

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    MenuWidget(DatabaseManager& dbHandler, QStackedWidget *stackedWidget, QWidget *parent = nullptr);
    ~MenuWidget() = default;

    // void setUsername(const QString &username);
    void setUsers(const QMap<QString, QPair<QString, QString>> &userMap) { users = userMap; }
    void setCurrentUser(const QString &username, const QString &email) { currentUser = qMakePair(username, email); }
    void setGroupChats(const QStringList &chats) { groupChats = chats; }

    // Chat action methods
    void startPrivateChat();
    void viewGroupChats();
    void createGroupChat();
    void joinGroupChat();
    void logoutRequested();

signals:
    void startPrivateChatRequested();
    void viewGroupChatsRequested();
    void createGroupChatRequested();
    void joinGroupChatRequested();

private:
    void setupUI();

    // UI components
    QLabel *titleLabel;
    QLabel *welcomeLabel;
    QPushButton *startPrivateChatButton;
    QPushButton *viewGroupChatsButton;
    QPushButton *createGroupChatButton;
    QPushButton *joinGroupChatButton;
    QPushButton *logoutButton;

    // Data members
    QStackedWidget *stackedWidget;
    QMap<QString, QPair<QString, QString>> users; // Map of email -> (username, password)
    QPair<QString, QString> currentUser; // Currently logged in user, (name, email)
    QStringList groupChats;

    group_chat_widget *groupChat;
    group_chat_list_widget  *groupChatListWidget;

    // Add the database handler
    DatabaseManager &dbHandler;
};

#endif // MENUWIDGET_H
