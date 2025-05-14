#ifndef GROUP_CHAT_LIST_H
#define GROUP_CHAT_LIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QScrollArea>
#include <QTabWidget>
#include "database_manager.h"

class group_chat_list_widget : public QWidget
{
    Q_OBJECT

public:
    explicit group_chat_list_widget(DatabaseManager &db_handler, const QString &user_email, QWidget *parent = nullptr);
    void refresh_group_lists();

signals:
    void back_to_menu_requested();
    void group_chat_selected(const QString &group_id);

private slots:
    void on_group_item_clicked(QListWidgetItem *item);
    void on_edit_group_name_clicked();
    void on_delete_group_clicked();

private:
    void setup_ui();
    void load_created_groups();
    void load_joined_groups();
    void setup_tab_style();
    void add_group_item_with_edit_button(const QString &group_id, const QString &group_name, int member_count, QListWidget *list_widget);


    DatabaseManager &db_handler;
    QString user_email;

    // UI Components
    QLabel *title_label;
    QPushButton *back_button;
    QTabWidget *tab_widget;
    QListWidget *created_groups_list_widget;
    QListWidget *joined_groups_list_widget;

};

#endif
