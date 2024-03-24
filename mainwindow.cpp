#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QModelIndexList"
#include <QScreen>
#include <vector>
#include <map>
#include <random>
map<QString, vector<QString>> grammar;
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
        ui->listWidget_lines->addItem("what");
        ui->listWidget_lines->addItem("wht");
        ui->listWidget_lines->addItem("w?");
        ui->listWidget_lines->addItem("whaaaat");
        break;
    }
    }
    u++;
    ui->pushButton_stage_1->setEnabled(true);
}


void MainWindow::on_pushButton_stage_1_clicked()
{
    ui->listWidget_stage_1->clear();
    ui->listWidget_residual_rules->clear();
    grammar.clear();
    rr_rules.clear();
    res_rules.clear();
    residual_rules.clear();
    vector<QString> strings;
    int max_size = 0;
    for (int i = 0; i < ui->listWidget_lines->count(); i++) {
        strings.push_back(ui->listWidget_lines->item(i)->text());
        if (max_size < strings.at(i).size())
            max_size = strings.at(i).size();
    }
    int pr_num = 1;
    for (int i = 0; i < strings.size(); i++) {
        if (strings.at(i).size() == max_size) {
            QString key = "";
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
    }
    for (auto iter : grammar) {
        QString str1 = iter.first;
        QString str2 = iter.second.at(0);
        for (int i = 1; i < iter.second.size(); i++)
            str2 += " | " + iter.second.at(i);
        ui->listWidget_stage_1->addItem(str1 + " -> " + str2);
    }
    ui->pushButton_stage_1->setEnabled(false);
    ui->pushButton_stage_2->setEnabled(true);
}

bool exists(QString str, vector<QString> arr) {
    bool fl = false;
    for (int i = 0; i < arr.size(); i++) {
        if (str == arr[i]) {
            fl = true;
            break;
        }
    }
    return fl;
}

void MainWindow::step2() {
    for (int i = 0; i < residual_rules.size(); i++) {
        vector<QString> arr = grammar[res_rules[i]];
        for (int j = 0; j < arr.size(); j++) {
            if (arr[j] == residual_rules[i])
                arr.erase(arr.begin() + j);
        }
        if (arr.empty())
            grammar.erase(res_rules[i]);
        else
            grammar[res_rules[i]] = arr;
    }
    for (int i = 0; i < rr_rules.size(); i++) {
        vector<QString> arr = grammar[rr_rules[i]];
        for (int j = 0; j < arr.size(); j++) {
            QString str = "";
            for (int k = 1; k < arr.at(j).size(); k++)
                str += arr.at(j).at(k);
            if (str == res_rules[i])
                arr[j] = arr.at(j).at(0) + rr_rules[i];
        }
        grammar[rr_rules[i]] = arr;
    }
    for (int i = 0; i < res_rules.size(); i++) {
        auto iter = grammar.find(res_rules[i]);
        if (iter != grammar.end()) {
            vector<QString> arr1 = grammar.at(res_rules[i]);
            vector<QString> arr2 = grammar.at(rr_rules[i]);
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
        QString str2 = iter.second.at(0);
        for (int i = 1; i < iter.second.size(); i++)
            str2 += " | " + iter.second.at(i);
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
            bool fl1 = true;
            for (int i = 0; i < iter1->second.size(); i++) {
                QString str1 = "";
                for (int j = 1; j < iter1->second.at(i).size(); j++) {
                    str1 += iter1->second.at(i).at(j);
                }
                if (str1 != iter1->first && str1 != "") {
                    fl1 = false;
                    break;
                }
            }
            if (fl1) {
                for (auto iter2 = next(iter1); iter2 != grammar.end(); ++iter2) {
                    if (iter1->second.size() == iter2->second.size()) {
                        bool fl2 = true;
                        for (int i = 0; i < iter2->second.size(); i++) {
                            QString str2 = "";
                            for (int j = 1; j < iter2->second.at(i).size(); j++) {
                                str2 += iter2->second.at(i).at(j);
                            }
                            if (str2 != iter2->first && str2 != "") {
                                fl2 = false;
                                break;
                            }
                        }
                        if (fl2) {
                            for (int i = 0; i < iter2->second.size(); i++) {
                                fl2 = false;
                                for (int j = 0; j < iter1->second.size(); j++) {
                                    if (iter2->second.at(i).at(0) == iter1->second.at(j).at(0)
                                        && ( (iter2->second.at(i).size() > 1 & iter1->second.at(j).size() > 1)
                                        || (iter2->second.at(i).size() == 1 & iter1->second.at(j).size() == 1)
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
                    QString str = "";
                    for (int j = 1; j < iter->second.at(i).size(); j++)
                        str += iter->second.at(i).at(j);
                    bool fl = false;
                    for (int i = 1; i < grammar_c.size(); i++)
                        if (grammar_c.at(i) == str) {
                            fl = true;
                            break;
                        }
                    if (fl) {
                        //if (iter->second.size() > 1) {
                            iter->second.at(i) = iter->second.at(i).at(0) + grammar_c.at(0);
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
        for (auto iter1 = grammar.begin(); iter1 != grammar.end(); ++iter1) {
            for (auto iter2 = next(iter1); iter2 != grammar.end(); ++iter2) {
                if (iter1->second.size() == iter2->second.size()) {
                    bool fl2;
                    for (int i = 0; i < iter2->second.size(); i++) {
                        fl2 = false;
                        for (int j = 0; j < iter1->second.size(); j++) {
                            if (iter2->second.at(i).at(0) == iter1->second.at(j).at(0)
                                && ( (iter2->second.at(i).size() > 1 & iter1->second.at(j).size() > 1)
                                    || (iter2->second.at(i).size() == 1 & iter1->second.at(j).size() == 1) )) {
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
                            QString str2 = "";
                            for (int j = 1; j < iter2->second.at(i).size(); j++)
                                str2 += iter2->second.at(i).at(j);
                            QString str1 = "";
                            for (int j = 1; j < iter1->second.at(i).size(); j++)
                                str1 += iter1->second.at(i).at(j);
                            if (iter2->first == str2 && iter2->first == str1) {
                                iter1->second[i] = iter1->second.at(i).at(0) + iter1->first;
                                wasChanged = true;
                            }
                            if (iter1->first == str1 && iter1->first == str2) {
                                iter2->second[i] = iter2->second.at(i).at(0) + iter2->first;
                                wasChanged = true;
                            }
                            if (iter1->first == str2 && iter2->first == str1) {
                                iter1->second[i] = iter1->second.at(i).at(0) + iter1->first;
                                iter2->second[i] = iter2->second.at(i).at(0) + iter2->first;
                                wasChanged = true;
                            }
                        }
                    }
                }
            }
        }
        if (wasChanged)
            checkOver();
    }
    return true;
}
bool vectorIncludes(const vector<QString>& haystack, const vector<QString>& needle) {
    for (const QString& needleElement : needle) {
        bool found = false;
        for (const QString& haystackElement : haystack) {
            if (haystackElement == needleElement) {
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
            map<QString, vector<QString>> gr = grammar;
            //step2();
            checkOver();
            checkOver2();
            if (gr == grammar) {
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
                vect.clear();
            }
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
        QString str2 = iter.second.at(0);
        for (int i = 1; i < iter.second.size(); i++)
            str2 += " | " + iter.second.at(i);
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
        vector<QString> vect = grammar[str];
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, vect.size() - 1);
        int rNum = dis(gen);
        tree_str += str + " -> " + vect.at(rNum) + "; " ;
        if (vect.at(rNum).size() == 1) {
            out_str += vect.at(rNum).at(0);
            break;
        }
        else {
            out_str += vect.at(rNum).at(0);
            str = "";
            for (int i = 1; i < vect.at(rNum).size(); i++)
                str += vect.at(rNum).at(i);
        }
    }
    out_str += "        " + tree_str;
    ui->listWidget_gen->addItem(out_str);
}

