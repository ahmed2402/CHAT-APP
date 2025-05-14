#ifndef GROUP_CHAT_VIEW_H
#define GROUP_CHAT_VIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QSplitter>
#include <QFrame>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QScrollBar>
#include <QMenu>
#include <QTimer>
#include <tuple>
#include <QWidgetAction>


#include "database_manager.h"

class group_chat_widget : public QWidget
{
    Q_OBJECT

public:
    explicit group_chat_widget(DatabaseManager &dbHandler, QString groupId, QPair<QString, QString> currentUser, QWidget *parent = nullptr);
    ~group_chat_widget();

    void setGroupName(const QString &name);
    QString getGroupName() const;
    void setGroupId(const QString &id) {groupId = id;}
    void setGroupAdmin(const QPair<QString, QString> & groupAdmin){this->groupAdmin = groupAdmin;}
    void setMembersList();
    void addMember(const QString &username);
    void removeMember(const QString &username);
    void clearChatHistory();
    void loadChatHistory();
    void addSystemMessage(const QString &message, QDateTime msgTimestamp = QDateTime::currentDateTime());

    void addIncomingMessage(const QString &sender, const QString &email, const QString &message, QDateTime msgTimestamp);

    void addOutgoingMessage(const QString &message, QDateTime msgTimestamp = QDateTime::currentDateTime());
    void updateMembersHeader();

signals:
    void backRequested();
    void messageSubmitted(const QString &message);

private slots:
    void sendMessage();
    void showMembersMenu();
    void leaveChatRequested();
    void handleMemberClicked(QListWidgetItem *item);

private:
    void setupUI();
    void setupConnections();

    QLabel *groupNameLabel;
    QPushButton *backButton;
    QPushButton *leaveChatButton;
    QPushButton *membersButton;
    QMenu *membersMenu;

    QTextEdit *chatHistoryDisplay;
    QLineEdit *messageInputField;
    QPushButton *sendMessageButton;
    QListWidget *membersListWidget;
    QMessageBox *confirmBox;
    QMessageBox *errorBox;
    QPushButton *addMemberButton;


    QPair<QString, QString> currentUser; // name, email
    QPair<QString, QString> groupAdmin; // name, email
    QString currentGroupName;
    QString groupId;
    QString formatTimestamp(const QDateTime &timestamp);
    void showAddMemberDialog();
    void addNewMemberToGroup(const QString &userId);

    DatabaseManager &dbHandler;
    QTimer *refreshTimer;
};

#endif // GROUP_CHAT_VIEW_H
