// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtTest/qtest.h>
#include <QtTest/qsignalspy.h>
#include <QtTest/qtesttouch.h>
#include <QtQuick/private/qquickrectangle_p.h>
#include <QtQuickTemplates2/private/qquickdockpanel_p.h>
#include <QtQuickTemplates2/private/qquickdocklayout_p.h>
#include <QtQuickTemplates2/private/qquickdockarea_p.h>
#include <QtQuickTemplates2/private/qquickdockarea_p_p.h>
#include <QtQuickTemplates2/private/qquickdockcontainer_p.h>
#include <QtQuickTemplates2/private/qquickdockcontainer_p_p.h>
#include <QtQuickTemplates2/private/qquickdocksidecontainer_p.h>
#include <QtQuick/qquickview.h>

#include <QtGui/qtestsupport_gui.h>
#include <QtQuickTestUtils/private/qmlutils_p.h>
#include <QtQuickTestUtils/private/viewtestutils_p.h>
#include <QtQuickTestUtils/private/visualtestutils_p.h>
#include <QtQuickControlsTestUtils/private/qtest_quickcontrols_p.h>
#include <QStyleHints>

class QQuickTextField;
using namespace QQuickVisualTestUtils;

class tst_dock : public QQmlDataTest
{
    Q_OBJECT

public:
    tst_dock();

private slots:
    void testDefaultValues();
    void testLayout();
    void testVisibility();
    void testParents();
    void testEdges();
    void testLayoutSize();
    void testVisibilityTransitions();
};

tst_dock::tst_dock() : QQmlDataTest(QT_QMLTEST_DATADIR) { }

void tst_dock::testDefaultValues()
{
    auto dock = new QQuickDockPanel;

    QCOMPARE(dock->isOpened(), true);
    QCOMPARE(dock->isActive(), false);
    QCOMPARE(dock->visibility(), QQuickDockPanel::Pinned);
}

void tst_dock::testLayout()
{
    QQuickApplicationHelper helper(this, QStringLiteral("Layout.qml"));
    auto window = helper.window;
    window->show();
    QVERIFY(QTest::qWaitForWindowExposed(window));

    const auto &container = window->property("container").value<QQuickDockContainer *>();
    QVERIFY(container != nullptr);

    const auto &layout = window->property("layout1").value<QQuickDockLayout *>();
    QVERIFY(layout);

    const auto &widget1 = window->property("widget1").value<QQuickDockPanel *>();
    widget1->togglePin();
}

void tst_dock::testVisibility()
{
    QQuickApplicationHelper helper(this, QStringLiteral("Visibility.qml"));
    auto window = helper.window;
    window->show();
    QVERIFY(QTest::qWaitForWindowExposed(window));

    const auto &container = window->property("container").value<QQuickDockContainer *>();
    QVERIFY(container != nullptr);

    auto containerPrivate = QQuickDockContainerPrivate::get(container);
    QVERIFY(containerPrivate);

    const auto &widget1 = window->property("widget1").value<QQuickDockPanel *>();
    const auto &group1 = window->property("group1").value<QQuickDockGroup *>();
    const auto &group2 = window->property("group2").value<QQuickDockGroup *>();
    const auto &layout1 = window->property("layout1").value<QQuickDockLayout *>();

    QVERIFY(widget1);
    QVERIFY(group1);
    QVERIFY(group2);
    QVERIFY(layout1);

    QCOMPARE(widget1->parentItem(), group1->contentItem());

    widget1->setEdge(Qt::TopEdge);
    widget1->setVisibility(QQuickDockPanel::Unpinned);
    QCOMPARE(widget1->parentItem(),
             QQuickDockAreaPrivate::get(containerPrivate->area(Qt::TopEdge))
                     ->sideContainer->contentItem());

    widget1->setVisibility(QQuickDockPanel::Detached);
    QCOMPARE(widget1->parentItem(), container->contentItem());

    widget1->setVisibility(QQuickDockPanel::Pinned);

    widget1->close();
    QCOMPARE(widget1->parentItem(), nullptr);

    widget1->open();
    QCOMPARE(widget1->parentItem(), group1->contentItem());
}

void tst_dock::testParents()
{
    QQuickApplicationHelper helper(this, QStringLiteral("Parents.qml"));
    auto window = helper.window;
    window->show();
    QVERIFY(QTest::qWaitForWindowExposed(window));

    const auto &container = window->property("container").value<QQuickDockContainer *>();
    QVERIFY(container != nullptr);

    auto containerPrivate = QQuickDockContainerPrivate::get(container);
    QVERIFY(containerPrivate);

    const auto &widget1 = window->property("widget1").value<QQuickDockPanel *>();
    const auto &widget2 = window->property("widget2").value<QQuickDockPanel *>();
    const auto &widget3 = window->property("widget3").value<QQuickDockPanel *>();
    const auto &widget4 = window->property("widget4").value<QQuickDockPanel *>();
    const auto &group1 = window->property("group1").value<QQuickDockGroup *>();

    QVERIFY(widget1);
    QVERIFY(widget2);
    QVERIFY(widget3);
    QVERIFY(widget4);
    QVERIFY(group1);

    QCOMPARE(widget1->parentItem(), group1->contentItem());
    auto areaPrivate = QQuickDockAreaPrivate::get(containerPrivate->area(Qt::TopEdge));
    QCOMPARE(widget2->parentItem(), areaPrivate->sideContainer->contentItem());
    QCOMPARE(widget3->parentItem(), container->contentItem());
    QCOMPARE(widget4->parentItem(), group1->contentItem());

    QVERIFY(widget1->isOpened());
    QVERIFY(widget2->isOpened());
    QVERIFY(widget3->isOpened());
    QVERIFY(!widget4->isOpened());
}

void tst_dock::testEdges()
{
    QQuickApplicationHelper helper(this, QStringLiteral("Unpinned.qml"));
    auto window = helper.window;
    window->show();
    QVERIFY(QTest::qWaitForWindowExposed(window));

    const auto &container = window->property("container").value<QQuickDockContainer *>();
    QVERIFY(container != nullptr);

    auto containerPrivate = QQuickDockContainerPrivate::get(container);
    QVERIFY(containerPrivate);

    const auto &widgetTop = window->property("widgetTop").value<QQuickDockPanel *>();
    const auto &widgetLeft = window->property("widgetLeft").value<QQuickDockPanel *>();
    const auto &widgetRight = window->property("widgetRight").value<QQuickDockPanel *>();
    const auto &widgetBottom = window->property("widgetBottom").value<QQuickDockPanel *>();

    QVERIFY(widgetTop);
    QVERIFY(widgetLeft);
    QVERIFY(widgetRight);
    QVERIFY(widgetBottom);

    QCOMPARE(widgetTop->visibility(), QQuickDockPanel::Visibility::Unpinned);

    auto areaTop = QQuickDockAreaPrivate::get(containerPrivate->area(Qt::TopEdge));
    auto areaLeft = QQuickDockAreaPrivate::get(containerPrivate->area(Qt::LeftEdge));
    auto areaRight = QQuickDockAreaPrivate::get(containerPrivate->area(Qt::RightEdge));
    auto areaBottom = QQuickDockAreaPrivate::get(containerPrivate->area(Qt::BottomEdge));

    QVERIFY(areaTop);
    QVERIFY(areaLeft);
    QVERIFY(areaRight);
    QVERIFY(areaBottom);

    QCOMPARE(widgetTop->parentItem(), areaTop->sideContainer->contentItem());
    QCOMPARE(widgetLeft->parentItem(), areaLeft->sideContainer->contentItem());
    QCOMPARE(widgetRight->parentItem(), areaRight->sideContainer->contentItem());
    QCOMPARE(widgetBottom->parentItem(), areaBottom->sideContainer->contentItem());
}

void tst_dock::testLayoutSize()
{
    QQuickApplicationHelper helper(this, QStringLiteral("LayoutSize.qml"));
    auto window = helper.window;
    window->show();
    QVERIFY(QTest::qWaitForWindowExposed(window));

    const auto &container = window->property("container").value<QQuickDockContainer *>();
    QVERIFY(container != nullptr);

    auto containerPrivate = QQuickDockContainerPrivate::get(container);
    QVERIFY(containerPrivate);

    const auto &widget1 = window->property("widget1").value<QQuickDockPanel *>();
    const auto &group1 = window->property("group1").value<QQuickDockGroup *>();
    const auto &widget2 = window->property("widget2").value<QQuickDockPanel *>();
    const auto &group2 = window->property("group2").value<QQuickDockGroup *>();
    const auto &mainItem = window->property("mainItem").value<QQuickRectangle *>();
    const auto &layout1 = window->property("layout1").value<QQuickDockLayout *>();

    QVERIFY(widget1);
    QVERIFY(group1);
    QVERIFY(widget2);
    QVERIFY(group2);
    QVERIFY(mainItem);
    QVERIFY(widget1->width() > 0);
    QVERIFY(widget2->width() > 0);

    QCOMPARE(group1->height(), layout1->height());
    QCOMPARE(group2->height(), layout1->height());

    QCOMPARE(group1->width(), group2->width());
}

void tst_dock::testVisibilityTransitions()
{
    QQuickApplicationHelper helper(this, QStringLiteral("Visibility.qml"));
    QQuickWindow *window = helper.window;
    window->show();
    QVERIFY(QTest::qWaitForWindowExposed(window));

    auto *container = window->property("container").value<QQuickDockContainer *>();
    QVERIFY(container);

    auto *containerPrivate = QQuickDockContainerPrivate::get(container);
    QVERIFY(containerPrivate);

    auto *widget1 = window->property("widget1").value<QQuickDockPanel *>();
    auto *group1 = window->property("group1").value<QQuickDockGroup *>();
    QVERIFY(widget1);
    QVERIFY(group1);

    // Helper lambda برای خواندن sideContainer مربوط به یک edge
    auto sideContainerOf = [&](Qt::Edge edge) -> QQuickItem * {
        auto *areaPriv = QQuickDockAreaPrivate::get(containerPrivate->area(edge));
        return areaPriv ? areaPriv->sideContainer->contentItem() : nullptr;
    };

    // ---------- شروع: باید Pinned باشد ----------
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Pinned);
    QCOMPARE(widget1->parentItem(), group1->contentItem());
    QVERIFY(widget1->isOpened());

    // ---------- 1. Pinned → Unpinned (Top) ----------
    widget1->setEdge(Qt::TopEdge);
    widget1->setVisibility(QQuickDockPanel::Unpinned);
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Unpinned);
    QCOMPARE(widget1->parentItem(), sideContainerOf(Qt::TopEdge));
    QVERIFY(widget1->isOpened());

    // ---------- 2. Unpinned → Detached ----------
    widget1->setVisibility(QQuickDockPanel::Detached);
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Detached);
    QCOMPARE(widget1->parentItem(), container->contentItem());
    QVERIFY(widget1->isOpened());

    // ---------- 3. Detached → Pinned ----------
    widget1->setVisibility(QQuickDockPanel::Pinned);
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Pinned);
    QCOMPARE(widget1->parentItem(), group1->contentItem());
    QVERIFY(widget1->isOpened());

    // ---------- 4. Pinned → Detached (مسیر مستقیم) ----------
    widget1->setVisibility(QQuickDockPanel::Detached);
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Detached);
    QCOMPARE(widget1->parentItem(), container->contentItem());

    // ---------- 5. Detached → Unpinned (Left) ----------
    widget1->setEdge(Qt::LeftEdge);
    widget1->setVisibility(QQuickDockPanel::Unpinned);
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Unpinned);
    QCOMPARE(widget1->parentItem(), sideContainerOf(Qt::LeftEdge));

    // ---------- 6. Unpinned → Pinned ----------
    widget1->setVisibility(QQuickDockPanel::Pinned);
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Pinned);
    QCOMPARE(widget1->parentItem(), group1->contentItem());

    // ---------- 7. Pinned → Closed ----------
    widget1->close();
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Closed);
    QCOMPARE(widget1->parentItem(), nullptr);
    QVERIFY(!widget1->isOpened());

    // ---------- 8. Closed → Pinned (open) ----------
    widget1->open();
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Pinned);
    QCOMPARE(widget1->parentItem(), group1->contentItem());
    QVERIFY(widget1->isOpened());

    // ---------- 9. Closed از حالت Unpinned ----------
    widget1->setEdge(Qt::RightEdge);
    widget1->setVisibility(QQuickDockPanel::Unpinned);
    widget1->close();
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Closed);
    QCOMPARE(widget1->parentItem(), nullptr);

    // ---------- 10. Closed → Unpinned (با open + setVisibility) ----------
    // رفتار فعلی open() معمولاً به Pinned برمی‌گرداند، پس بعد از open دوباره Unpinned می‌کنیم
    widget1->open();
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Pinned); // فرض بر اساس تست فعلی
    widget1->setEdge(Qt::BottomEdge);
    widget1->setVisibility(QQuickDockPanel::Unpinned);
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Unpinned);
    QCOMPARE(widget1->parentItem(), sideContainerOf(Qt::BottomEdge));

    // ---------- 11. Closed از حالت Detached ----------
    widget1->setVisibility(QQuickDockPanel::Detached);
    widget1->close();
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Closed);
    QCOMPARE(widget1->parentItem(), nullptr);

    // بازگشت نهایی
    widget1->open();
    QCOMPARE(widget1->visibility(), QQuickDockPanel::Pinned);
    QCOMPARE(widget1->parentItem(), group1->contentItem());
}

QTEST_QUICKCONTROLS_MAIN(tst_dock)

#include "tst_dock.moc"
