#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QModelIndexList"
#include <QScreen>
#include <vector>
#include <map>
#include <random>
struct Production {
    QString terminal = "";
    QString non_terminal = "";
    // Перегрузка оператора "==" для сравнения Production на равенство
    bool operator==(const Production& other) const {
        return (terminal == other.terminal) && (non_terminal == other.non_terminal);
    }
};

map<QString, vector<Production>> grammar;
vector<QString> residual_rules;
vector<QString> res_rules;
vector<QString> rr_rules;
int u = 0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QScreen*> screens = QGuiApplication::screens();
    QScreen *screen = screens.at(0);
    setFixedSize(screen->size().width(), screen->size().height());
    move(0, 0);
    ui->pushButton_gen->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_delete_line_clicked()
{
    ui->listWidget_lines->model()->removeRow(0);
    ui->pushButton_stage_1->setEnabled(true);
    ui->pushButton_gen->setEnabled(false);
    ui->listWidget_gen->clear();
}


void MainWindow::on_pushButton_add_lines_ready_clicked()
{
    ui->listWidget_lines->clear();
    ui->listWidget_gen->clear();
    ui->pushButton_gen->setEnabled(false);
    switch (u % 5) {
    case 0: {
        ui->listWidget_lines->addItem("cb");
        ui->listWidget_lines->addItem("bbb");
        ui->listWidget_lines->addItem("cab");
        ui->listWidget_lines->addItem("bbab");
        ui->listWidget_lines->addItem("caab");
        ui->listWidget_lines->addItem("caaab");
        ui->listWidget_lines->addItem("bbaab");
        break;
    }
    case 1: {
        ui->listWidget_lines->addItem("cc");
        ui->listWidget_lines->addItem("cb");
        ui->listWidget_lines->addItem("caab");
        ui->listWidget_lines->addItem("caaab");
        ui->listWidget_lines->addItem("db");
        ui->listWidget_lines->addItem("dd");
        ui->listWidget_lines->addItem("daaab");
        break;
    }
    case 2: {
        ui->listWidget_lines->addItem("sd");
        ui->listWidget_lines->addItem("smad");
        ui->listWidget_lines->addItem("pmd");
        ui->listWidget_lines->addItem("pd");
        ui->listWidget_lines->addItem("pmmmad");
        ui->listWidget_lines->addItem("smammd");
        break;
    }
    case 3: {
        ui->listWidget_lines->addItem("hl");
        ui->listWidget_lines->addItem("fd");
        ui->listWidget_lines->addItem("heeeel");
        ui->listWidget_lines->addItem("ffeeed");
        break;
    }
    case 4: {
        ui->listWidget_lines->addItem("what?");
        ui->listWidget_lines->addItem("wht?");
        ui->listWidget_lines->addItem("w?");
        ui->listWidget_lines->addItem("whaaaat?");
        break;
    }
    }
    u++;
    ui->pushButton_stage_1->setEnabled(true);
}

// Функция для сравнения строк по длине (для сортировки)
bool compareByLength(const QString& a, const QString& b) {
    return a.size() > b.size(); // Сортировка по убыванию длины
}

void MainWindow::on_pushButton_stage_1_clicked()
{

    ui->listWidget_stage_1->clear();
    ui->listWidget_residual_rules->clear();
    grammar.clear();
    residual_rules.clear();
    res_rules.clear();
    rr_rules.clear();
    vector<QString> strings;
    int max_size = 0;
    for (int i = 0; i < ui->listWidget_lines->count(); i++) {
        strings.push_back(ui->listWidget_lines->item(i)->text());
        if (max_size < strings.at(i).size())
            max_size = strings.at(i).size();
    }
    // Сортировка вектора по длине строк
    sort(strings.begin(), strings.end(), compareByLength);
    for (int i = 0; i < strings.size(); i++) {
        QString strToCompare = strings.at(i);
        for (int j = i + 1; j < strings.size(); j++)
            if (strToCompare == strings.at(j))
                strings.erase(strings.begin() + j);
    }
    int max_str_num = 0;
    while (max_str_num < strings.size() && strings.at(max_str_num).size() == max_size)
        max_str_num++;
    int i = 0;
    int pr_num = 1;
    while (i < max_str_num) {
        QString key = "S";
        QString two_last = "";
        for (int ii = strings.at(i).size() - 2; ii > -1 && ii < strings.at(i).size(); ii++)
            two_last += strings.at(i).at(ii);
        bool fl = true;
        for (int j = 0; j < strings.at(i).size() && fl; j++) {
            if (strings.at(i).size() - 2 == j) {
                for (int k = max_str_num; k < strings.size() && fl; k++) {
                        QString two_last1 = "";
                        for (int ii = strings.at(k).size() - 2; ii > -1 && ii < strings.at(k).size(); ii++)
                            two_last1 += strings.at(k).at(ii);

                        if (two_last1 != two_last)
                            fl = false;
                }
            }
            if (fl) {
                if (j == 0)
                    key = "S";
                else
                    key = "A" + QString::number(pr_num - 1);
                vector<Production> arr = grammar[key];
                Production str_add;
                str_add.terminal = strings.at(i).at(j);
                if (j != strings.at(i).size() - 1)
                    str_add.non_terminal = "A" + QString::number(pr_num);
                else
                    pr_num--;
                arr.push_back(str_add);
                grammar[key] = arr;
                pr_num++;
            }
        }
        if (!fl) {
            rr_rules.push_back(key);
            key = "A" + QString::number(pr_num - 1);
            vector<Production> arr;
            Production str_add;
            for (int j = strings.at(i).size() - 2; j < strings.at(i).size() && j > -1; j++)
                str_add.terminal += strings.at(i).at(j);
            arr.push_back(str_add);
            residual_rules.push_back(str_add.terminal);
            res_rules.push_back(key);
            ui->listWidget_residual_rules->addItem("Остаточное правило: " + key + " -> " + str_add.terminal);
            grammar[key] = arr;
        }
        i++;
    }
    i = max_str_num;
    while (i < strings.size()) {
        QString key = "S";
        for (int j = 0; j < strings.at(i).size(); j++) {
            vector<Production> arr = grammar[key];
            bool fl = false;
            for (int k = 0; k < arr.size(); k++) {
                if ( (arr.at(k).terminal == strings.at(i).at(j) && arr.at(k).non_terminal != "")
                    || (arr.at(k).terminal == strings.at(i).at(j) && arr.at(k).non_terminal == "" && j == (strings.at(i).size() - 1)) ) {
                    fl = true;
                    key = arr.at(k).non_terminal;
                    break;
                }
            }
            if (!fl) {
                if (j == strings.at(i).size() - 1) {
                    Production str_add;
                    str_add.terminal = strings.at(i).at(j);
                    arr.push_back(str_add);
                    grammar[key] = arr;
                }
                else {
                    Production str_add;
                    str_add.non_terminal = "A" + QString::number(pr_num);
                    str_add.terminal = strings.at(i).at(j);
                    arr.push_back(str_add);
                    grammar[key] = arr;
                    key = "A" + QString::number(pr_num);
                    pr_num++;
                }
            }
        }
        i++;
    }
    int uu = 0;
    /*int pr_num = 1;
    for (int i = 0; i < strings.size(); i++) {
        if (strings.at(i).size() == max_size) {
            QString key = "S";
            bool fl = true;
            for (int j = 0; j < strings.at(i).size() && fl; j++) {
                if (strings.at(i).size() - 2 == j) {
                    for (int k = 0; k < strings.size() && fl; k++) {
                        if (k != i) {
                            QString two_last = "";
                            QString two_last_tr = "";
                            for (int ii = strings.at(k).size() - 2; ii > -1 && ii < strings.at(k).size(); ii++)
                                two_last += strings.at(k).at(ii);
                            for (int ii = strings.at(i).size() - 2; ii > -1 && ii < strings.at(i).size(); ii++)
                                two_last_tr += strings.at(i).at(ii);
                            if (two_last != two_last_tr)
                                fl = false;
                        }
                    }
                }
                if (fl) {
                    if (j == 0)
                        key = "S";
                    else
                        key = "A" + QString::number(pr_num - 1);
                    vector<QString> arr = grammar[key];
                    QString str_add = strings.at(i).at(j);
                    if (j != strings.at(i).size() - 1)
                        str_add += "A" + QString::number(pr_num);
                    else
                        pr_num--;
                    arr.push_back(str_add);
                    grammar[key] = arr;
                    pr_num++;
                }
            }
            if (!fl) {
                rr_rules.push_back(key);
                key = "A" + QString::number(pr_num - 1);
                vector<QString> arr;
                QString str_add = "";
                for (int j = strings.at(i).size() - 2; j < strings.at(i).size() && j > -1; j++)
                    str_add += strings.at(i).at(j);
                arr.push_back(str_add);
                residual_rules.push_back(str_add);
                res_rules.push_back(key);
                ui->listWidget_residual_rules->addItem("Остаточное правило: " + key + " -> " + str_add);
                grammar[key] = arr;
            }
        }
    }
    for (int i = 0; i < strings.size(); i++) {
        if (strings.at(i).size() != max_size) {
            QString prod = "S";
            vector<QString> arr = grammar[prod];
            for (int j = 0; j < strings.at(i).size(); j++) {
                bool fl = false;
                for (int k = 0; k < arr.size() && !fl; k++) {
                    if (arr.at(k).at(0) == strings.at(i).at(j) && arr.at(k).size() != 2) {
                        prod = "";
                        for (int n = 1; n < arr.at(k).size(); n++) {
                            prod += arr.at(k).at(n);
                        }
                        if (prod.size() > 1)
                            arr = grammar[prod];
                            fl = true;
                    }
                }
                if (!fl) {
                    arr.push_back(strings.at(i).at(j));
                    grammar[prod] = arr;
                }
            }
        }
    }*/
    for (auto iter : grammar) {
        QString str1 = iter.first;
        QString str2 = iter.second.at(0).terminal + iter.second.at(0).non_terminal;
        for (int i = 1; i < iter.second.size(); i++)
            str2 += " | " + iter.second.at(i).terminal + iter.second.at(i).non_terminal;
        ui->listWidget_stage_1->addItem(str1 + " -> " + str2);
    }
    ui->pushButton_stage_1->setEnabled(false);
    ui->pushButton_stage_2->setEnabled(true);
}

bool exists(Production str, vector<Production> arr) {
    bool fl = false;
    for (int i = 0; i < arr.size(); i++) {
        if (str.non_terminal == arr[i].non_terminal && str.terminal == arr[i].terminal) {
            fl = true;
            break;
        }
    }
    return fl;
}

void MainWindow::step2() {
    /*for (auto iter : grammar) {
        for (int i = 0; i < iter.second.size(); i++) {

        }
    }*/
    for (int i = 0; i < residual_rules.size(); i++) {
        vector<Production> arr = grammar[res_rules[i]];
        for (int j = 0; j < arr.size(); j++) {
            if (arr[j].terminal == residual_rules[i])
                arr.erase(arr.begin() + j);
        }
        if (arr.empty())
            grammar.erase(res_rules[i]);
        else
            grammar[res_rules[i]] = arr;
    }
    for (int i = 0; i < rr_rules.size(); i++) {
        vector<Production> arr = grammar[rr_rules[i]];
        for (int j = 0; j < arr.size(); j++) {
            if (arr.at(j).non_terminal == res_rules[i])
                arr[j].non_terminal = rr_rules[i];
        }
        grammar[rr_rules[i]] = arr;
    }
    for (int i = 0; i < res_rules.size(); i++) {
        auto iter = grammar.find(res_rules[i]);
        if (iter != grammar.end()) {
            vector<Production> arr1 = grammar.at(res_rules[i]);
            vector<Production> arr2 = grammar.at(rr_rules[i]);
            for (int j = 0; j < arr1.size(); j++) {
                if (!exists(arr1.at(j), arr2))
                    arr2.push_back(arr1.at(j));
            }
            grammar[rr_rules[i]] = arr2;
            grammar.erase(res_rules[i]);
        }
    }
}

void MainWindow::on_pushButton_stage_2_clicked()
{
    step2();
    ui->listWidget_stage_2->clear();
    for (auto iter : grammar) {
        QString str1 = iter.first;
        QString str2 = iter.second.at(0).terminal + iter.second.at(0).non_terminal;
        for (int i = 1; i < iter.second.size(); i++)
            str2 += " | " + iter.second.at(i).terminal + iter.second.at(i).non_terminal;
        ui->listWidget_stage_2->addItem(str1 + " -> " + str2);
    }
    ui->pushButton_stage_2->setEnabled(false);
    ui->pushButton_stage_3->setEnabled(true);
}

bool MainWindow::checkOver() {
    bool wasChanged = true;
    while (wasChanged) {
        wasChanged = false;
        vector<QString> grammar_c;
        for (auto iter1 = grammar.begin(); iter1 != grammar.end(); ++iter1) {
            bool fl1 = false;
            for (int i = 0; i < iter1->second.size(); i++) {
                if (iter1->second.at(i).non_terminal == iter1->first/*iter1->second.at(i).non_terminal != iter1->first && iter1->second.at(i).non_terminal != ""*/) {
                    fl1 = true;
                    break;
                }
            }
            if (fl1) {
                for (auto iter2 = next(iter1); iter2 != grammar.end(); ++iter2) {
                    if (iter1->second.size() == iter2->second.size()) {
                        bool fl2 = false;
                        for (int i = 0; i < iter2->second.size(); i++) {
                            if (iter2->second.at(i).non_terminal == iter2->first/*iter2->second.at(i).non_terminal != iter2->first && iter2->second.at(i).non_terminal != ""*/) {
                                fl2 = true;
                                break;
                            }
                        }
                        if (fl2) {
                            for (int i = 0; i < iter2->second.size(); i++) {
                                fl2 = false;
                                for (int j = 0; j < iter1->second.size(); j++) {
                                    if (iter2->second.at(i).terminal == iter1->second.at(j).terminal
                                        && ( (iter2->second.at(i).non_terminal != "" & iter1->second.at(j).non_terminal != "")
                                        || (iter2->second.at(i).non_terminal == "" & iter1->second.at(j).non_terminal == "")
                                            /*|| (iter2->second.at(i).size() == 1 & iter1->second.at(i).size() == 1)*/ )  ) {
                                        fl2 = true;
                                        break;
                                    }
                                }
                                if (fl2 == false) {
                                    break;
                                }
                            }
                            if (fl2) {
                                wasChanged = true;
                                grammar_c.push_back(iter1->first);
                                grammar_c.push_back(iter2->first);
                            }
                        }
                    }
                }
            }
        }
        if (wasChanged) {
            for (int i = 0; i < grammar_c.size(); i++) {
                for (int j = i + 1; j < grammar_c.size(); j++) {
                    if (grammar_c.at(i) == grammar_c.at(j)) {
                        grammar_c.erase(grammar_c.begin() + j);
                        j--;
                    }
                }
            }
            for (auto iter = grammar.begin(); iter != grammar.end(); ++iter) {
                for (int i = 0; i < iter->second.size(); i++) {
                    bool fl = false;
                    for (int j = 1; j < grammar_c.size(); j++)
                        if (grammar_c.at(j) == iter->second.at(i).non_terminal) {
                            fl = true;
                            break;
                        }
                    if (fl) {
                        //if (iter->second.size() > 1) {
                            iter->second.at(i).non_terminal = grammar_c.at(0);
                            grammar[iter->first] = iter->second;
                        //}
                    }
                }
            }
            for (int i = 1; i < grammar_c.size(); i++)
                grammar.erase(grammar_c.at(i));
        }
    }
    return true;
}

bool MainWindow::checkOver2() {
    bool wasChanged = true;
    while (wasChanged) {
        wasChanged = false;
        vector<QString> toDel;
        for (auto iter1 = grammar.begin(); iter1 != grammar.end(); ++iter1) {
            for (auto iter2 = next(iter1); iter2 != grammar.end(); ++iter2) {
                if (iter1->second.size() == iter2->second.size()) {
                    bool fl2;
                    for (int i = 0; i < iter2->second.size(); i++) {
                        fl2 = false;
                        for (int j = 0; j < iter1->second.size(); j++) {
                            if (iter2->second.at(i).terminal == iter1->second.at(j).terminal
                                && ( (iter2->second.at(i).non_terminal != "" & iter1->second.at(j).non_terminal != "")
                                    || (iter2->second.at(i).non_terminal == "" & iter1->second.at(j).non_terminal == "")
                                    || iter1->second == iter2->second )) {
                                fl2 = true;
                                break;
                            }
                        }
                        if (fl2 == false) {
                            break;
                        }
                    }
                    if (fl2) {
                        for (int i = 0; i < iter2->second.size(); i++) {
                            if (iter2->first == iter2->second.at(i).non_terminal && iter2->first == iter1->second.at(i).non_terminal) {
                                iter1->second[i].non_terminal = iter1->first;
                                wasChanged = true;
                            }
                            if (iter1->first == iter1->second.at(i).non_terminal && iter1->first == iter2->second.at(i).non_terminal) {
                                iter2->second[i].non_terminal = iter2->first;
                                wasChanged = true;
                            }
                            if (iter1->first == iter2->second.at(i).non_terminal && iter2->first == iter1->second.at(i).non_terminal) {
                                iter1->second[i].non_terminal = iter1->first;
                                iter2->second[i].non_terminal = iter2->first;
                                wasChanged = true;
                            }
                            if (iter1->second == iter2->second) {
                                for (auto iter11 = grammar.begin(); iter11 != grammar.end(); ++iter11) {
                                    for (int i = 0; i < iter11->second.size(); i++)
                                        if (iter11->second.at(i).non_terminal == iter2->first) {
                                            iter11->second.at(i).non_terminal = iter1->first;
                                            grammar[iter11->first] = iter11->second;
                                        }
                                }
                                toDel.push_back(iter2->first);
                                wasChanged = true;
                            }
                        }
                    }
                }
            }
        }
        if (wasChanged) {
            for (int i = 0; i < toDel.size(); i++)
                grammar.erase(toDel.at(i));
            checkOver();
            toDel.clear();
        }
    }
    return true;
}
bool vectorIncludes(const vector<Production>& haystack, const vector<Production>& needle) {
    for (const Production& needleElement : needle) {
        bool found = false;
        for (const Production& haystackElement : haystack) {
            if (haystackElement.non_terminal == needleElement.non_terminal && haystackElement.terminal == needleElement.terminal) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}
bool MainWindow::checkOver3() {
    bool wasChanged = true;
    vector<QString> vect;
    int ie = 0;
    while (wasChanged) {
        wasChanged = false;
        for (auto iter1 = grammar.begin(); iter1 != grammar.end(); ++iter1) {
            bool fl = false;
            for (auto iter2 = next(iter1); iter2 != grammar.end(); ++iter2) {
                if (vectorIncludes(iter2->second, iter1->second)) {
                    iter1->second = iter2->second;
                    wasChanged = true;
                }
                if (vectorIncludes(iter1->second, iter2->second)) {
                    iter2->second = iter1->second;
                    wasChanged = true;
                }
                if (vectorIncludes(iter2->second, iter1->second) && vectorIncludes(iter1->second, iter2->second)) {
                    vect.push_back(iter1->first);
                    vect.push_back(iter2->first);
                    fl = true;
                    break;
                }
            }
            if (fl) {
                break;
            }
        }
        if (wasChanged) {
            map<QString, vector<Production>> gr = grammar;
            //step2();
            checkOver();
            checkOver2();
            bool flag = false;
            for (auto iter1 = grammar.begin(); iter1 != grammar.end(); ++iter1) {
                for (auto iter2 = gr.begin(); iter2 != gr.end(); ++iter2) {
                    if (iter1 != iter2)
                        flag = true;
                }
            }
            if (flag) {
                ie++;
            }
            if (!flag) {
                /*for (auto& iter : grammar) {
                    for (int i = 0; i < iter.second.size(); i++) {
                        QString str = "";
                        for (int j = 1; j < iter.second.at(i).size(); j++)
                            str += iter.second.at(i).at(j);
                        if (str == vect.at(1)) {
                            iter.second.at(i) = iter.second.at(i).at(0) + vect.at(0);
                        }
                    }
                }
                grammar.erase(vect.at(1));*/
                break;
            }
            vect.clear();
        }
    }
    return true;
}

void MainWindow::on_pushButton_stage_3_clicked()
{
    ui->listWidget_stage_3->clear();
    checkOver();
    checkOver2();
    checkOver3();
    for (auto iter : grammar) {
        QString str1 = iter.first;
        QString str2 = iter.second.at(0).terminal + iter.second.at(0).non_terminal;
        for (int i = 1; i < iter.second.size(); i++)
            str2 += " | " + iter.second.at(i).terminal + iter.second.at(i).non_terminal;
        ui->listWidget_stage_3->addItem(str1 + " -> " + str2);
    }
    ui->pushButton_stage_3->setEnabled(false);
    ui->pushButton_gen->setEnabled(true);
    ui->listWidget_gen->clear();
}


void MainWindow::on_pushButton_gen_clicked()
{
    QString str = "S";
    QString out_str = "";
    QString tree_str = "";
    while (1) {
        vector<Production> vect = grammar[str];
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, vect.size() - 1);
        int rNum = dis(gen);
        tree_str += str + " -> " + vect.at(rNum).terminal + vect.at(rNum).non_terminal + "; " ;
        if (vect.at(rNum).non_terminal == "") {
            out_str += vect.at(rNum).terminal;
            break;
        }
        else {
            out_str += vect.at(rNum).terminal;
            str = vect.at(rNum).non_terminal;
        }
    }
    out_str += "        " + tree_str;
    ui->listWidget_gen->addItem(out_str);
}
