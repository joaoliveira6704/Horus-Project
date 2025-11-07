#include <QApplication>
#include <QMainWindow>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QFontDatabase>
#include <QDebug>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "attitudeindicator.h"

class PFDMainWindow : public QMainWindow {
    Q_OBJECT

public:
    PFDMainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("UAV Primary Flight Display");
        resize(1024, 768);
        // Central widget
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // Top info bar
        QHBoxLayout *topBar = new QHBoxLayout();
        altLabel = new QLabel("ALT: 0 m", this);
        speedLabel = new QLabel("SPD: 0 m/s", this);
        headingLabel = new QLabel("HDG: 0°", this);

        // Use custom font for labels
        QFont labelFont(nimbusMono, 16);
        altLabel->setFont(labelFont);
        speedLabel->setFont(labelFont);
        headingLabel->setFont(labelFont);

        topBar->addWidget(altLabel);
        topBar->addStretch();
        topBar->addWidget(speedLabel);
        topBar->addStretch();
        topBar->addWidget(headingLabel);

        mainLayout->addLayout(topBar);

        // Attitude indicator (main display)
        attitudeIndicator = new AttitudeIndicator(this);
        attitudeIndicator->setMinimumSize(1000, 1000);

        mainLayout->addWidget(attitudeIndicator, 1, Qt::AlignCenter);

        // Bottom status bar
        statusLabel = new QLabel("Status: Simulating flight data", this);
        statusLabel->setFont(QFont("Arial", 10));
        mainLayout->addWidget(statusLabel);

        // Simulation timer (replace with real telemetry later)
        simTimer = new QTimer(this);
        connect(simTimer, &QTimer::timeout, this, &PFDMainWindow::updateSimulation);
        simTimer->start(20); // 20 Hz update rate

        simTime = 0.0;

        // Set fonts to AttitudeIndicator
        attitudeIndicator->setCustomFonts(PFDMainWindow::customFontFamily, nimbusMono);
    }

    // Static member to hold font family name
    static QString customFontFamily;
    static QString nimbusMono;

private slots:
    void updateSimulation() {
        // Simulate flight data (replace with real telemetry parsing)
        simTime += 0.02;

        // Simulate gentle banking and pitching
        float pitch = 90.0f * std::sin(simTime * 0.2);
        float roll = 180.0f * std::sin(simTime * 0.5);
        float speed = 70.0f + 230.0f * std::sin(simTime * 0.4);
        float heading = std::fmod(simTime * 10.0, 360.0);
        // Simulate other parameters
        float altitudeRate = 2000.0f;
        float batteryState = 4.2f + 1.0f * std::sin(simTime * 5);
        // Quantity of propellers in uav (for rpm quantity)
        int propQuantity = 4;
        int rpm[4] = {
            static_cast<int>(2500 + 1560.0f * std::sin(simTime * 0.2)),
            static_cast<int>(2500 + 1210.0f * std::sin(simTime * 0.25)),
            static_cast<int>(2500 + 1543.0f * std::sin(simTime * 0.27)),
            static_cast<int>(2500 + 1673.0f * std::sin(simTime * 0.29))
        };
        float QNH = 29.92f;
        float altitude = 8500.00f + 1 * std::sin(simTime * 0.2);
        float OAT = 8.0f;
        std::string flightMode = "ATLC Takeoff Active";
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        std::stringstream ss;
        ss << std::put_time(localTime, "%H:%M:%S");
        std::string timeStr = ss.str();

        // Update attitude indicator
        attitudeIndicator->setAttitude(pitch, roll, altitude, speed, heading,QNH, flightMode, timeStr, rpm, batteryState, propQuantity, OAT);

        // Update displays
        altLabel->setText(QString("ALT: %1 ft").arg(altitude, 0, 'f', 1));
        speedLabel->setText(QString("SPD: %1 kts").arg(speed, 0, 'f', 1));
        headingLabel->setText(QString("Roll: %1°").arg(roll, 0, 'f', 0));
    }

private:
    AttitudeIndicator *attitudeIndicator;
    QLabel *altLabel;
    QLabel *speedLabel;
    QLabel *headingLabel;
    QLabel *statusLabel;
    QTimer *simTimer;
    double simTime;
};

// Define static member
QString PFDMainWindow::customFontFamily = "Courier";
QString PFDMainWindow::nimbusMono = "Nimbus Mono PS";

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Load custom font
    int fontId1 = QFontDatabase::addApplicationFont(":/fonts/armarurgt.ttf");int fontId2 = QFontDatabase::addApplicationFont(":/fonts/NimbusMono.otf");
    QString nimbusMonoFamily = "Nimbus Mono PS"; // fallback
    if (fontId1 != -1) {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId2);
        if (!families.isEmpty()) {
            nimbusMonoFamily = families.at(0);
            qDebug() << "NimbusMono loaded" << nimbusMonoFamily;
        }
    } else {
        PFDMainWindow::customFontFamily = "Courier";
    }
    if (fontId2 != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId2);
        if (!fontFamilies.isEmpty()) {
            PFDMainWindow::nimbusMono = fontFamilies.at(0);
            qDebug() << "Loaded NimbusMono font" << PFDMainWindow::nimbusMono;
        }
    } else {
        PFDMainWindow::nimbusMono = "Nimbus Mono PS";
    }

    PFDMainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"