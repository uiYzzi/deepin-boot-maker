/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "resultview.h"
#include "widgetutil.h"
#include "deviceinfoitem.h"
#include "devicelistwidget.h"
#include "dwaterprogress.h"
#include <bminterface.h>
#include <backend/bmhandler.h>

#include <DListWidget>
#include <DApplication>
#include <DCheckBox>
#include <DLog>
#include <DApplicationHelper>

#include <QDebug>
#include <QUrl>
#include <QVBoxLayout>
#include <QProcess>
#include <QDesktopServices>

ResultView::ResultView(DWidget *parent) : DWidget(parent)
{
    setObjectName("ResultView");
    setAutoFillBackground(true);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);
    mainLayout->setSpacing(0);

    m_title = new DLabel(tr("Successful"));
    m_title->setFixedHeight(35);
    QFont qf = m_title->font();
    qf.setFamily("SourceHanSansSC-Medium");
    qf.setPixelSize(24);
    m_title->setFont(qf);

    m_resultIcon = new DLabel();
    m_resultIcon->setObjectName("ResultIcon");
    m_resultIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/success.svg", this));

    m_hitsTitle = new DLabel();
    m_hitsTitle->setObjectName("ResulteHitsTitle");
    m_hitsTitle->setFixedWidth(340);
    m_hitsTitle->setFixedHeight(25);

    qf = m_hitsTitle->font();
    qf.setFamily("SourceHanSansSC-Bold");
    qf.setPixelSize(17);
    qf.setBold(true);
    m_hitsTitle->setFont(qf);
    m_hitsTitle->setAlignment(Qt::AlignCenter);

//    QString hitsFormat = "<a style='color:#b4b4b4; font-size:11px'>%1</a>";
//    QString tagBegin = "<a href='#show_log'><span style='text-decoration: underline; color:#1B85ff;'>";
//    QString tagEnd = "</span></a>";
//    QString log = tr("Installation logs are stored in %1HERE%2, you can upload to forum to help us solve your problem.");
    m_logHits = new DLabel(/*hitsFormat.arg(log.arg(tagBegin).arg(tagEnd))*/);
    m_logHits->setObjectName("ResultErrorDescription");
    m_logHits->setWordWrap(true);
    m_logHits->setFixedWidth(400);
    m_logHits->setFixedHeight(34);
//    connect(m_logHits, &QLabel::linkActivated, this, &ResultView::onLogLinkActivated);
//    m_logHits->setOpenExternalLinks(false);
    m_logHits->hide();
    qf = m_logHits->font();
    qf.setFamily("SourceHanSansSC-Normal");
    qf.setPixelSize(12);
    m_logHits->setFont(qf);
    m_logHits->setAlignment(Qt::AlignCenter);

//    m_rebootLater = new SuggestButton();
    m_rebootLater = new DPushButton();
    m_rebootLater->setFocusPolicy(Qt::NoFocus);
    m_rebootLater->setFixedSize(310, 36);
    m_rebootLater->setObjectName("RebootLater");
    m_rebootLater->setText(tr("Done"));
    m_rebootLater->setProperty("normal", true);/*
    rebootLater->style()->unpolish(rebootLater);
    rebootLater->style()->polish(rebootLater);*/

//    m_rebootNow = new SuggestButton();
    m_rebootNow = new DPushButton();
    m_rebootNow->setFocusPolicy(Qt::NoFocus);
    m_rebootNow->setFixedSize(310, 36);
    m_rebootNow->setObjectName("RebootLater");
    m_rebootNow->setText(tr("Reboot now"));

    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(87);
    mainLayout->addWidget(m_resultIcon, 0, Qt::AlignCenter);
    mainLayout->addSpacing(11);
    mainLayout->addWidget(m_hitsTitle, 0, Qt::AlignCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(m_logHits, 0, Qt::AlignCenter);
    mainLayout->addStretch(113);
    mainLayout->addWidget(m_rebootLater, 0, Qt::AlignCenter);
//    mainLayout->addSpacing(15);
//    m_rebootNow->hide();
//    mainLayout->addWidget(m_rebootNow, 0, Qt::AlignCenter);


    connect(m_rebootNow, &DPushButton::clicked,
    this, [ = ]() {
        BMInterface::instance()->reboot();
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
    this, [ = ] {
        DPalette pa;
        DGuiApplicationHelper::ColorType themeType = DGuiApplicationHelper::instance()->themeType();
        if (themeType == DGuiApplicationHelper::LightType)
        {
            pa = palette();
            pa.setColor(DPalette::Background, QColor(255, 255, 255));
            setPalette(pa);
            pa = m_title->palette();
            pa.setColor(DPalette::Text, QColor("#001A2E"));
            m_title->setPalette(pa);
            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::Text, QColor("#001A2E"));
            m_hitsTitle->setPalette(pa);
            pa = m_logHits->palette();
            pa.setColor(DPalette::Text, QColor("#526A7F"));
            m_logHits->setPalette(pa);
        } else if (themeType == DGuiApplicationHelper::DarkType)
        {
            pa = palette();
            pa.setColor(DPalette::Background, QColor("#252525"));
            setPalette(pa);
            pa = m_title->palette();
            pa.setColor(DPalette::Text, QColor("#C0C6D4"));
            m_title->setPalette(pa);
            pa = m_hitsTitle->palette();
            pa.setColor(DPalette::Text, QColor("#C0C6D4"));
            m_hitsTitle->setPalette(pa);
            pa = m_logHits->palette();
            pa.setColor(DPalette::Text, QColor("#6D7C88"));
            m_logHits->setPalette(pa);
        }
    });

    emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());

}

void ResultView::updateResult(quint32 error, const QString &/*title*/, const QString &/*description*/)
{
    auto errorType = static_cast<BMHandler::ErrorType>(error);

    switch (errorType) {
    case BMHandler::NoError:
        m_hitsTitle->setText(tr("Restart the computer from the U disk boot can experienc"));
        m_rebootLater->disconnect();
        connect(m_rebootLater, &DPushButton::clicked,
        this, [ = ]() {
            qApp->exit(0);
        });
        return;
    case BMHandler::SyscExecFailed:
        m_logHits->setText(tr("The error log will be uploaded automatically with the feedback. We cannot improve without your feedback"));
        m_rebootLater->setText(tr("Submit Feedback"));
        m_logHits->adjustSize();
        m_rebootLater->disconnect();
        connect(m_rebootLater, &DPushButton::clicked,
        this, [ = ]() {
            // FIXME: call feedback
            QProcess::startDetached("deepin-feedback");
        });
        break;
    case BMHandler::USBFormatError:
    case BMHandler::USBSizeError:
    case BMHandler::USBMountFailed:
    case BMHandler::ExtractImgeFailed:
        m_logHits->setText(BMHandler::errorString(errorType));
        m_rebootLater->setText(tr("Close"));
        m_rebootLater->disconnect();
        connect(m_rebootLater, &DPushButton::clicked,
        this, [ = ]() {
            qApp->exit(0);
        });
        break;
    }
    m_hitsTitle->setText(tr("Sorry, process failed"));
    m_title->setText(tr("Process failed"));
    m_resultIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/light/image/fail.svg", this));
    m_logHits->show();
    m_rebootNow->hide();
}

void ResultView::onLogLinkActivated(const QString &link)
{
    if (link == "#show_log") {
        QDesktopServices::openUrl(QUrl::fromLocalFile(Dtk::Core::DLogManager::getlogFilePath()));
    }
}
