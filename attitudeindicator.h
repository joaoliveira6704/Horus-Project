#ifndef ATTITUDEINDICATOR_H
#define ATTITUDEINDICATOR_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <cmath>

class AttitudeIndicator : public QWidget {
    Q_OBJECT

public:
    explicit AttitudeIndicator(QWidget *parent = nullptr)
    : QWidget(parent), pitch(0.0f), roll(0.0f),
      customFontFamily("Courier"), nimbusMono("Arial") {
        setMinimumSize(1000, 1000);
    }


    void setAttitude(float pitchDeg, float rollDeg, float altFt, float speedKts, float headingDeg,float qnhVal, std::string fltMode, std::string timeCurr, const int rpmVal[4], float batteryStateVal, int propQuantityVal, float
        oatVal) {
        pitch = -pitchDeg;
        roll = rollDeg;
        altitude = altFt;
        speed = speedKts;
        heading = headingDeg;
        flightMode = fltMode;
        currTime = timeCurr;
        QNH = qnhVal;
        OAT = oatVal;
        for (int i = 0; i < 4; i++) {
            rpm[i] = rpmVal[i];
        }
        batteryState = batteryStateVal;
        propQuantity = propQuantityVal;
        update(); // Trigger repaint
    }

    void setCustomFonts(const QString &font1, const QString &font2) {
        customFontFamily = font1;
        nimbusMono = font2;
        update();
    }


protected:
    float zoom = 6.0f;
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        // Fill entire widget with black
        painter.fillRect(rect(), Qt::black);
        int side = qMin(width(), height());
        painter.setViewport((width() - side) / 2, (height() - side) / 2, side, side);
        painter.setWindow(-100, -100, 200, 200);

        // Draw sky and ground
        drawHorizon(painter);

        // Draw pitch ladder
        drawPitchLadder(painter);

        // Draw roll indicator
        drawRollIndicator(painter);

        // Draw center aircraft symbol
        drawAircraftSymbol(painter);

        drawAltitudeTape(painter);

        drawSpeedTape(painter);

        drawHeadingTape(painter);

        drawFlightMode(painter);

        drawClock(painter);

        drawGauges(painter);

        drawQNH(painter);
    }

private:
    void drawHorizon(QPainter &painter) {
        painter.save();

        // Rotate for roll
        painter.rotate(-roll);

        // Calculate vertical offset for pitch (pixels per degree)
        float pitchOffset = pitch * zoom;

        // Draw horizon line extending across entire display
        painter.setPen(QPen(Qt::green, 1));

        // Main center segments (between aircraft symbol and tapes)
        painter.drawLine(-200, -pitchOffset, -13, -pitchOffset);
        painter.drawLine(13, -pitchOffset, 200, -pitchOffset);

        painter.restore();
    }

    void drawPitchLadder(QPainter &painter) {
        painter.save();

        // Rotate for roll
        painter.rotate(-roll);

        painter.setPen(QPen(Qt::green, 0.5));
        painter.setFont(QFont(customFontFamily, 3));

        // Draw pitch lines every 5 degrees
        for (int angle = -90; angle <= 90; angle += 5) {
            if (angle == 0) continue; // Skip horizon line

            // The ladder is fixed in world space
            // Positive angles are above horizon (negative y), negative angles below
            float y = - (angle * zoom + pitch * zoom);

            // Only draw if visible
            if (y < -75 || y > 100) continue;

            if (angle > 0) {
                QPen posPen(Qt::green, 0.5);
                posPen.setStyle(Qt::SolidLine);
                painter.setPen(posPen);
                painter.drawLine(-15, y, -30, y);
                painter.drawLine(15, y, 30, y);
                painter.drawLine(-30, y, -30, y+3);
                painter.drawLine(30, y, 30, y+3);
                // Draw angle text
                QString text = QString::number(angle);
                painter.drawText(-27, y + 3.05, text);
                painter.drawText(25, y + 3.05, text);
            }
            else {
                QPen negPen(Qt::green, 0.5);
                negPen.setStyle(Qt::DashLine);
                negPen.setDashPattern({2.0,4.0});
                painter.setPen(negPen);

                painter.drawLine(-15, y, -30, y+2);
                painter.drawLine(15, y, 30, y+2);
                negPen.setStyle(Qt::SolidLine);
                painter.setPen(negPen);
                painter.drawLine(-15, y, -15, y-3);
                painter.drawLine(15, y, 15, y-3);
                // Draw angle text
                QString text = QString::number(angle);
                painter.drawText(-21, y-0.5, text);
                painter.drawText(16, y-0.5, text);
            }
        }

        painter.restore();
    }

    void drawRollIndicator(QPainter &painter) {
        painter.save();

        const int radius = 42;
        const int tickLong = 2;     // (long ticks)
        const int tickShort = 1;  //  (short ticks)
        const int pointerHeight = 3;
        const float pointerWidth = 2.5f;
        const float centerMarkWidth = 2.5f;
        const int centerMarkHeight = 6;

        // Draw roll scale arc (flipped upward)
        painter.setPen(QPen(Qt::green, 0.5));

        //Draw arc
        /*painter.drawArc(-radius, -radius, 2*radius, 2*radius, 210 * 16, 120 * 16);*/

        // Draw roll marks (flipped vertically)
        for (int angle = -20; angle <= 20; angle += 10) {
            painter.save();
            painter.rotate(angle);
            painter.drawLine(0, radius - tickShort, 0, radius); // short ticks 10,20
            painter.restore();
        }

        for (int angle = -30; angle <= 30; angle += 60) {
            painter.save();
            painter.rotate(angle);
            painter.drawLine(0, radius - tickLong, 0, radius); // long ticks 30
            painter.restore();
        }

        for (int angle = -45; angle <= 45; angle += 90) {
            painter.save();
            painter.rotate(angle);
            painter.drawLine(0, radius - tickLong - 2, 0, radius); // longer ticks 45
            painter.restore();
        }

        //Limit roll to 45 degrees
        double clampedRoll = std::clamp(static_cast<double>(roll), -45.0, 45.0);

        // Draw roll pointer
        painter.save();
        painter.rotate(-clampedRoll);

        QPainterPath triangleSmall;
        triangleSmall.moveTo(0, radius);
        triangleSmall.lineTo(-pointerWidth, radius + pointerHeight);
        triangleSmall.lineTo(pointerWidth, radius + pointerHeight);
        triangleSmall.closeSubpath();

        painter.setBrush(Qt::green);
        painter.drawPath(triangleSmall);
        QPainterPath triangleOuter;
        triangleOuter.moveTo(0, radius);
        triangleOuter.lineTo(-pointerWidth-2, radius + pointerHeight +2);
        triangleOuter.lineTo(pointerWidth+2, radius + pointerHeight +2);
        triangleOuter.closeSubpath();
        painter.setBrush(Qt::transparent);
        painter.drawPath(triangleOuter);
        painter.restore();

        // Draw center reference mark (flipped vertically)
        QPainterPath centerMark;
        centerMark.moveTo(0, radius - pointerHeight/2);
        centerMark.lineTo(-centerMarkWidth + 1.5, radius - pointerHeight / 2 - centerMarkHeight + 3);
        centerMark.lineTo(centerMarkWidth - 1.5, radius - pointerHeight / 2 - centerMarkHeight + 3);
        centerMark.closeSubpath();

        painter.setBrush(Qt::transparent);
        painter.drawPath(centerMark);

        painter.restore();
    }



    void drawAircraftSymbol(QPainter &painter) {
        painter.save();

        painter.setPen(QPen(Qt::green, 0.5));

        // Wings
        painter.drawLine(-12, 0, -6, 0);
        painter.drawLine(6, 0, 12, 0);
        //W shape
        painter.drawLine(-6, 0, -3, 3);
        painter.drawLine(6, 0, 3, 3);
        painter.drawLine(-3, 3, 0, 0);
        painter.drawLine(3, 3, 0, 0);

        painter.restore();
    }

    int calculateBaroAltitudeInt() {
        float hpaQNH = QNH * 33.865;
        float baroAlt = altitude + (hpaQNH - 1013.25f) * 30.0f;
        float ISA = 15.0f;
        float ISA_Temp = ISA - 2.0f * (baroAlt / 1000.0f);
        float ISA_Dev = OAT - ISA_Temp;
        float densityAltitude = (4.0f * (baroAlt / 1000.0f) * ISA_Dev) + baroAlt;

        return static_cast<int>(densityAltitude);
    }

    void drawAltitudeTape(QPainter &painter) {
        painter.save();
        // --- Layout constants ---
        const int tapeX = 60;         // Horizontal position
        const int tapeWidth = 12;     // Width of the tape
        const int tickSpacing = 10;    // Pixel distance between 100 ft ticks
        const int stepFt = 100;       // Feet per tick
        const int labelStep = 500;    // Label every 500 ft
        const int visibleRangeFt = 600; // Visible range above/below center

        // --- Appearance ---
        painter.setPen(QPen(Qt::green, 0.5));
        painter.setFont(QFont(nimbusMono, 3));

        // --- Background ---
        painter.setBrush(Qt::transparent);
        int tapeHeight = (visibleRangeFt / stepFt) * tickSpacing * 2;

        painter.setPen(QPen(Qt::green, 0.5));

        // Left line
        painter.drawLine(tapeX-5, tapeHeight/2, tapeX-5, -tapeHeight/2);
        // Bottom Line
        painter.drawLine(tapeX-5, tapeHeight/2, tapeX+tapeWidth, tapeHeight/2);
        // Top Line
        painter.drawLine(tapeX-5, -tapeHeight/2, tapeX+tapeWidth, -tapeHeight/2);
        int baroAltitude = calculateBaroAltitudeInt();
        for (int alt = -90000; alt <= 90000; alt += stepFt) {
            float y = (baroAltitude - alt) * (tickSpacing / (float)stepFt);

            if (y < -60 || y > 60) continue;

            // Draw tick
            painter.drawLine(tapeX - 4.5, y, tapeX, y);

            // Label only every 500 ft
            if (alt % labelStep == 0 and (alt != int(baroAltitude) || alt != int(baroAltitude-1))) {
                QString text = QString::number(alt);
                painter.drawText(tapeX + 2, y+1, text);
            }
        }

        if (baroAltitude >= 0) {
            // --- Draw current altitude box ---
            QRectF box(tapeX + 2, -3, 15, 6);
            painter.setBrush(Qt::black);
            painter.setPen(QPen(Qt::red, 0.5));
            painter.drawRect(box);
            painter.setPen(QPen(Qt::green, 0.5));
            QString text = QString::number(int(baroAltitude));
            painter.drawText(tapeX + 4, 1, text);
        }
        else {
            // --- Draw current altitude box ---
            QRectF box(tapeX + 2, -3, 20, 6);
            painter.setBrush(Qt::black);
            painter.setPen(QPen(Qt::red, 0.5));
            painter.drawRect(box);
            painter.setPen(QPen(Qt::green, 0.5));
            QString text = QString::number(abs(int(baroAltitude)));
            painter.drawText(tapeX + 4, 1, "NEG " + text);
        }
        painter.drawText(tapeX-3, -62, "BARO ALT (FEET)");
        painter.drawText(55, 65, "ALT AGL: " + QString::number(int(altitude)) + "FT");
        painter.restore();
    }

    void drawSpeedTape(QPainter &painter) {
        painter.save();
        // --- Layout constants ---
        const int tapeX = 60;         // Horizontal position
        const int tapeWidth = 12;     // Width of the tape
        const int tickSpacing = 10;    // Pixel distance between 100 ft ticks
        const int stepKts = 10;       // Feet per tick
        const int labelStep = 500;    // Label every 500 ft
        const int visibleRangeKts = 60; // Visible range above/below center

        // --- Appearance ---
        painter.setPen(QPen(Qt::green, 0.5));
        painter.setFont(QFont(nimbusMono, 3));

        // --- Background ---
        painter.setBrush(Qt::transparent);
        int tapeHeight = (visibleRangeKts / stepKts) * tickSpacing * 2;

        painter.setPen(QPen(Qt::green, 0.5));

        // right line
        painter.drawLine(-tapeX+5, tapeHeight/2, -tapeX+5, -tapeHeight/2);
        // Bottom Line
        painter.drawLine(-tapeX+5, tapeHeight/2, -tapeX-tapeWidth, tapeHeight/2);
        // Top Line
        painter.drawLine(-tapeX+5, -tapeHeight/2, -tapeX-tapeWidth, -tapeHeight/2);

        for (int spd = 0; spd <= 350; spd += stepKts) {
            float y = (speed - spd) * (tickSpacing / (float)stepKts);
            if (speed < 0) {
                speed = 0;
            }
            if (y < -60 || y > 60) continue;

            if (speed >= 0) {
                // Draw tick
                painter.drawLine(-tapeX + 4.5, y, -tapeX, y);
            }

            // Draw Current speed tick
            painter.drawLine(-tapeX + 4.5, 0, -tapeX, 0);

            // Label only every 500 ft
            if (int(speed-3) > spd || spd > int(speed+3)) {
                QString text = QString::number(spd);
                painter.drawText(-tapeX - 7, y+1, text);
            }
        }

        // --- Draw current altitude box ---
        QRectF box(-tapeX - 16, -3, 15, 6);
        painter.setBrush(Qt::black);
        painter.setPen(QPen(Qt::red, 0.5));
        painter.drawRect(box);
        painter.setPen(QPen(Qt::green, 0.5));
        QString text = QString::number(int(speed));
        painter.drawText(-tapeX - 7, 1, text);
        painter.drawText(-tapeX - 10, -62, "SPEED KTS");
        painter.restore();
    }

    void drawHeadingTape(QPainter &painter) {
    painter.save();
    // --- Layout constants ---
    const int tapeX = 45;         // Horizontal position
    const int tapeWidth = 90;     // Width of the tape
    const int tickSpacing = 10;    // Pixel distance between ticks
    const int stepDeg = 5;       // Degrees per tick
    const int labelStep = 10;    // Label every 10 degrees
    const int visibleRangeDeg = 50; // Visible range left/right center

    // --- Appearance ---
    painter.setPen(QPen(Qt::green, 0.5));
    painter.setFont(QFont(nimbusMono, 3));

    // --- Background ---
    painter.setBrush(Qt::transparent);
    int tapeHeight = 70;

    painter.setPen(QPen(Qt::green, 0.5));

    // Top Line
    painter.drawLine(-tapeX+5, -tapeHeight, tapeX-5, -tapeHeight);

    // Draw ticks
    for (int hdg = -visibleRangeDeg; hdg <= 360 + visibleRangeDeg; hdg += stepDeg) {
        // Normalize heading to 0-360 range
        int normalizedHdg = hdg % 360;
        if (normalizedHdg < 0) normalizedHdg += 360;

        // Calculate position relative to current heading
        float diff = normalizedHdg - heading;

        // Handle wrapping (shortest angular distance)
        if (diff > 180) diff -= 360;
        if (diff < -180) diff += 360;

        float x = diff * (tickSpacing / (float)stepDeg);

        // Only draw if visible
        if (x < -40 || x > 40) continue;

        float y = -tapeHeight;

        // Draw tick
        painter.drawLine(x, y, x, y - 5);

        // Label only every 10 degrees
        if (normalizedHdg % labelStep == 0) {
            QString text = QString("%1").arg(normalizedHdg / 10, 2, 10, QChar('0'));

            if (normalizedHdg / 10 == 0) text = "N";       // North
            else if (normalizedHdg == 90) text = "E";      // East
            else if (normalizedHdg == 180) text = "S";     // South
            else if (normalizedHdg == 270) text = "W";     // West

            painter.drawText(x - 2, y - 7, text);
        }
    }

    // --- Draw current heading box ---
    QRectF box(-5, -tapeHeight - 12, 9.5, 6);
    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(QPen(Qt::red, 0.5));
    painter.drawRect(box);
    painter.setPen(QPen(Qt::green, 0.5));

    // Normalize displayed heading
    int displayHeading = int(heading) % 360;
    if (displayHeading < 0) displayHeading += 360;

    QString text = QString::number(displayHeading);
    text = text.rightJustified(3, '0');
    painter.drawText(-3, -tapeHeight - 8, text);

    painter.restore();
}

    void drawFlightMode(QPainter &painter) {
        painter.save();

        painter.setPen(QPen(Qt::yellow, 0.5));
        painter.setFont(QFont(customFontFamily, 3));
        painter.drawText(-(flightMode.length()), -85, QString::fromStdString(flightMode));
    }

    void drawClock(QPainter &painter) {
        painter.save();

        painter.setPen(QPen(Qt::white, 0.5));
        painter.setFont(QFont(customFontFamily, 3));
        painter.drawText(-70, 90 - 8, "CLK (GMT)");
        painter.setPen(QPen(Qt::yellow, 0.5));
        painter.drawText(-69.75, 95 - 8, QString::fromLatin1(currTime));
    }

    void drawGauges(QPainter &painter) {
        painter.save();

        const int radius = 8;


        int oldPos = -47.5;
        for (int i=1; i<=propQuantity; i++) {
            painter.setPen(QPen(Qt::red, 0.5));
            painter.drawArc(-95, oldPos, 2*radius, 2*radius, 180 * 16, 270 * 16);
            painter.setPen(QPen(Qt::yellow, 0.5));
            painter.setFont(QFont(customFontFamily, 3));
            QString textRpm = QString::number(int(rpm[i-1]));
            painter.drawText(-92.5, oldPos + 20, "RPM #" + QString::number(i));
            painter.setPen(QPen(Qt::white, 0.5));
            painter.drawText(-90, oldPos + 10, textRpm);

            oldPos += 25;
        }

        /*// Draw roll marks (flipped vertically)
        for (int angle = -20; angle <= 20; angle += 10) {
            painter.save();
            painter.rotate(angle);
            painter.drawLine(75, -40, 70, -40); // short ticks 10,20
            painter.restore();
        }*/

        QString textBattery = QString::number(float(batteryState), 'f',1);
        painter.drawText(60, -70, "BATTERY:");
        painter.setPen(QPen(Qt::white, 0.5));
        painter.drawText(75, -70, textBattery + "V");

        painter.restore();
    }

    void drawQNH(QPainter &painter) {
        painter.save();

        painter.setPen(QPen(Qt::white, 0.5));
        QString textQNH = QString::number(float(QNH*33.865), 'f',2);
        painter.drawText(55, 70, "INHG");
        painter.setPen(QPen(Qt::yellow, 0.5));
        painter.drawText(65, 70, textQNH);

        painter.setPen(QPen(Qt::white, 0.5));
        QString textHPa = QString::number(float(QNH), 'f',2);
        painter.drawText(55, 75, "HPA");
        painter.setPen(QPen(Qt::yellow, 0.5));
        painter.drawText(65, 75, textHPa);

        painter.restore();
    }

private:
    float pitch; // degrees
    float roll;  // degrees
    float altitude;
    float speed;
    float heading;
    int rpm[4];
    float batteryState;
    int propQuantity;
    float QNH;
    float OAT;
    std::string flightMode;
    std::string currTime;
    QString customFontFamily;  // Custom font name
    QString nimbusMono;  // Custom font name
};

#endif // ATTITUDEINDICATOR_H