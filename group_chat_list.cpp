#include "group_chat_list.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFont>
#include <QIcon>

group_chat_list_widget::group_chat_list_widget(DatabaseManager &db_handler, const QString &user_email, QWidget *parent)
    : QWidget(parent), db_handler(db_handler), user_email(user_email)
{
    setup_ui();
    load_created_groups();
    load_joined_groups();
}

void group_chat_list_widget::setup_ui()
{
    // Main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setSpacing(15);
    main_layout->setContentsMargins(20, 20, 20, 20);

    // Set widget background and font
    setStyleSheet(
        "QWidget { background-color: #2d2d2d; color: #ffffff; }"
        "QWidget { font-family: 'Segoe UI', Arial, sans-serif; }"
        );

    // Header section
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // Back button with modern style
    back_button = new QPushButton("←", this);
    back_button->setStyleSheet(
        "QPushButton {"
        "   background-color: #363636;"
        "   border-radius: 15px;"
        "   padding: 8px;"
        "   font-size: 18px;"
        "   min-width: 30px;"
        "   max-width: 30px;"
        "   color: #ffffff;"
        "}"
        "QPushButton:hover { background-color: #404040; }"
        );
    connect(back_button, &QPushButton::clicked, this, &group_chat_list_widget::back_to_menu_requested);

    // Title with modern font
    title_label = new QLabel("Group Chats", this);
    title_label->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #ffffff;"
        "margin: 10px;"
        );
    title_label->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(back_button);
    headerLayout->addWidget(title_label, 1);
    main_layout->addLayout(headerLayout);

    // Tab widget for group lists
    tab_widget = new QTabWidget(this);
    setup_tab_style();

    // Created groups list
    created_groups_list_widget = new QListWidget(this);
    created_groups_list_widget->setStyleSheet(
        "QListWidget {"
        "   background-color: #363636;"
        "   border-radius: 10px;"
        "   border: none;"
        "   padding: 10px;"
        "}"
        "QListWidget::item {"
        "   background: transparent;"
        "   border-radius: 8px;"
        "   margin: 5px;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: rgba(74, 74, 74, 0.7);"
        "}"
        );

    // Joined groups list
    joined_groups_list_widget = new QListWidget(this);
    joined_groups_list_widget->setStyleSheet(
        "QListWidget {"
        "   background-color: #363636;"
        "   border-radius: 12px;"
        "   border: 1px solid #404040;"
        "   padding: 12px;"
        "}"
        "QListWidget::item {"
        "   background: transparent;"
        "   border-radius: 8px;"
        "   margin: 5px;"
        "   color: #ffffff;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: rgba(74, 74, 74, 0.7);"
        "}"
        );

    // Connect signals
    connect(created_groups_list_widget, &QListWidget::itemClicked, this, &group_chat_list_widget::on_group_item_clicked);
    connect(joined_groups_list_widget, &QListWidget::itemClicked, this, &group_chat_list_widget::on_group_item_clicked);

    // Add tabs
    tab_widget->addTab(created_groups_list_widget, "My Groups");
    tab_widget->addTab(joined_groups_list_widget, "Joined Groups");

    main_layout->addWidget(tab_widget);
}

void group_chat_list_widget::setup_tab_style()
{
    tab_widget->setStyleSheet(
        "QTabWidget::pane {"
        "   border: none;"
        "   background-color: #363636;"
        "   border-radius: 10px;"
        "   margin-top: -1px;"
        "}"
        "QTabBar::tab {"
        "   background-color: #404040;"
        "   color: #ffffff;"
        "   padding: 12px 25px;"
        "   margin-right: 2px;"
        "   border-top-left-radius: 8px;"
        "   border-top-right-radius: 8px;"
        "   font-weight: bold;"
        "}"
        "QTabBar::tab:selected {"
        "   background-color: #2196F3;"
        "}"
        "QTabBar::tab:hover:!selected {"
        "   background-color: #4a4a4a;"
        "}"
        );
}

void group_chat_list_widget::load_created_groups()
{
    created_groups_list_widget->clear();
    QList<std::tuple<QString, QString, int>> groups = db_handler.getCreatedGroups(user_email);
    for (const auto &group : groups) {
        QString id = std::get<0>(group);
        QString name = std::get<1>(group);
        int memberCount = std::get<2>(group);
        add_group_item_with_edit_button(id, name, memberCount, created_groups_list_widget);
    }
}

void group_chat_list_widget::load_joined_groups()
{
    joined_groups_list_widget->clear();
    QList<std::tuple<QString, QString, int>> groups = db_handler.getJoinedGroups(user_email);

    for (const auto &group : groups) {
        QString id = std::get<0>(group);
        QString name = std::get<1>(group);
        int memberCount = std::get<2>(group);

        QWidget *item_widget = new QWidget(joined_groups_list_widget);
        item_widget->setFixedHeight(50);
        item_widget->setStyleSheet(
            "QWidget {"
            "   background-color: #404040;"
            "   border-radius: 8px;"
            "}"
            );

        QHBoxLayout *layout = new QHBoxLayout(item_widget);
        layout->setContentsMargins(15, 5, 15, 5);
        layout->setSpacing(10);

        // Group icon
        QLabel *icon_label = new QLabel(item_widget);
        icon_label->setPixmap(QPixmap(":/icons/group.png").scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        layout->addWidget(icon_label);

        // Group info
        QVBoxLayout *info_layout = new QVBoxLayout();
        QLabel *name_label = new QLabel(name, item_widget);
        name_label->setStyleSheet("font-size: 14px; font-weight: bold; color: #ffffff;");

        QLabel *details_label = new QLabel(QString("ID: %1 • %2 members").arg(id).arg(memberCount), item_widget);
        details_label->setStyleSheet("font-size: 12px; color: #a0a0a0;");

        info_layout->addWidget(name_label);
        info_layout->addWidget(details_label);
        layout->addLayout(info_layout, 1);

        QListWidgetItem *item = new QListWidgetItem(joined_groups_list_widget);
        item->setSizeHint(item_widget->sizeHint());
        item->setData(Qt::UserRole, id);
        joined_groups_list_widget->setItemWidget(item, item_widget);
    }
}

void group_chat_list_widget::add_group_item_with_edit_button(const QString &groupId, const QString &groupName, int memberCount, QListWidget *listWidget)
{
    QWidget *item_widget = new QWidget(listWidget);
    item_widget->setFixedHeight(50);  // Set fixed height for consistency
    item_widget->setStyleSheet(
        "QWidget {"
        "   background-color: #404040;"
        "   border-radius: 8px;"
        "}"
        "QWidget:hover {"
        "   background-color: #505050;"
        "}"
        );
    item_widget->setCursor(Qt::PointingHandCursor);

    QHBoxLayout *main_layout = new QHBoxLayout(item_widget);
    main_layout->setContentsMargins(15, 5, 15, 5);
    main_layout->setSpacing(10);

    // Left side - Group info
    QVBoxLayout *info_layout = new QVBoxLayout();
    info_layout->setSpacing(2);
    info_layout->setContentsMargins(0, 0, 0, 0);

    // Group name and ID
    QLabel *name_label = new QLabel(groupName, item_widget);
    name_label->setStyleSheet(
        "QLabel {"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   padding: 0;"
        "   background: transparent;"
        "}"
        );
    info_layout->addWidget(name_label);

    // ID and member count in gray
    QLabel *details_label = new QLabel(QString("ID: %1  •  %2 members").arg(groupId).arg(memberCount), item_widget);
    details_label->setStyleSheet(
        "QLabel {"
        "   color: #a0a0a0;"
        "   font-size: 12px;"
        "   padding: 0;"
        "   background: transparent;"
        "}"
        );
    info_layout->addWidget(details_label);
    main_layout->addLayout(info_layout, 1);

    // Right side - Edit and Delete buttons
    QHBoxLayout *button_layout = new QHBoxLayout();
    button_layout->setSpacing(5);
    button_layout->setContentsMargins(0, 0, 0, 0);

    QPushButton *edit_button = new QPushButton("Edit", item_widget);
    edit_button->setProperty("groupName", groupName);
    edit_button->setCursor(Qt::PointingHandCursor);
    edit_button->setFixedSize(35, 22);
    edit_button->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 255, 255, 0.1);"
        "   border-radius: 3px;"
        "   padding: 0;"
        "   color: #ffffff;"
        "   font-size: 10px;"
        "   border: none;"
        "   margin: 0;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 0.2);"
        "}"
        );

    QPushButton *delete_button = new QPushButton("Delete", item_widget);
    delete_button->setProperty("groupId", groupId);
    delete_button->setCursor(Qt::PointingHandCursor);
    delete_button->setFixedSize(35, 22);
    delete_button->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 0, 0, 0.1);"
        "   border-radius: 3px;"
        "   padding: 0;"
        "   color: #ffffff;"
        "   font-size: 10px;"
        "   border: none;"
        "   margin: 0;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 0, 0, 0.2);"
        "}"
        );

    button_layout->addWidget(edit_button);
    button_layout->addWidget(delete_button);
    main_layout->addLayout(button_layout);

    connect(edit_button, &QPushButton::clicked, this, &group_chat_list_widget::on_edit_group_name_clicked);
    connect(delete_button, &QPushButton::clicked, this, &group_chat_list_widget::on_delete_group_clicked);

    QListWidgetItem *item = new QListWidgetItem(listWidget);
    item->setData(Qt::UserRole, groupId);
    item->setData(Qt::DisplayRole, groupName);  // Store the group name
    item->setSizeHint(QSize(item_widget->sizeHint().width(), 50));  // Fixed height
    listWidget->addItem(item);
    listWidget->setItemWidget(item, item_widget);
}

void group_chat_list_widget::on_edit_group_name_clicked()
{
    QPushButton *edit_button = qobject_cast<QPushButton*>(sender());
    if (!edit_button) return;

    QString oldGroupName = edit_button->property("groupName").toString();
    bool ok;
    QString newGroupName = QInputDialog::getText(this, "Edit Group Name",
                                                 "Enter new group name:",
                                                 QLineEdit::Normal,
                                                 oldGroupName, &ok);

    if (ok && !newGroupName.isEmpty() && newGroupName != oldGroupName) {
        // Update the group name in database
        // Note: You'll need to add this function to ChatDatabaseHandler
        if (db_handler.updateGroupName(oldGroupName, newGroupName)) {
            refresh_group_lists();
        } else {
            QMessageBox::warning(this, "Error", "Failed to update group name.");
        }
    }
}

void group_chat_list_widget::on_delete_group_clicked()
{
    QPushButton *delete_button = qobject_cast<QPushButton*>(sender());
    if (!delete_button) return;

    QString groupId = delete_button->property("groupId").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Group",
        "Are you sure you want to delete this group?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (db_handler.deleteGroup(groupId)) {
            refresh_group_lists();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete the group.");
        }
    }
}

void group_chat_list_widget::refresh_group_lists()
{
    load_created_groups();
    load_joined_groups();
}

void group_chat_list_widget::on_group_item_clicked(QListWidgetItem *item)
{
    QString groupId = item->data(Qt::UserRole).toString();
    emit group_chat_selected(groupId);
}
