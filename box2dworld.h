/*
 * Box2D QML plugin
 * Copyright (C) 2010 Nokia Corporation
 *
 * This file is part of the Box2D QML plugin.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library;  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOX2DWORLD_H
#define BOX2DWORLD_H

#include <QDeclarativeItem>
#include <QList>
#include <QBasicTimer>
#include <QTime>

class Box2DBody;
class Box2DFixture;
class Box2DJoint;
class ContactListener;
class Box2DDestructionListener;

class b2World;

/**
 * Default to 32 pixels in one meter, for static build you can set your own
 * TODO: Maybe turn this into a property of the world, though it can't be changed dynamically.
 */
#ifndef QML_BOX2D_SCALERATIO
#define QML_BOX2D_SCALERATIO (32.0f)
#endif
static const float scaleRatio = QML_BOX2D_SCALERATIO; 

/**
 * Wrapper class around a Box2D world.
 */
class Box2DWorld : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool allowSleeping READ getAllowSleeping WRITE setAllowSleeping NOTIFY allowSleepingChanged)
    Q_PROPERTY(float timeStep READ timeStep WRITE setTimeStep)
    Q_PROPERTY(int velocityIterations READ velocityIterations WRITE setVelocityIterations)
    Q_PROPERTY(int positionIterations READ positionIterations WRITE setPositionIterations)
    Q_PROPERTY(int frameTime READ frameTime WRITE setFrameTime)
    Q_PROPERTY(QPointF gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(bool reportFps READ reportFps WRITE setReportFps NOTIFY reportFpsChanged)
    Q_PROPERTY(float fps READ getFps NOTIFY fpsChanged)
    Q_PROPERTY(float scaleRatio READ getScaleRatio)

public:
    explicit Box2DWorld(QDeclarativeItem *parent = 0);
    ~Box2DWorld();

    /**
     * The amount of time to step through each frame in seconds.
     * By default it is 1 / 60.
     */
    float timeStep() const { return mTimeStep; }
    void setTimeStep(float timeStep) { mTimeStep = timeStep; }

    bool isRunning() const { return mIsRunning; }
    void setRunning(bool running);

    bool getAllowSleeping() const { return mAllowSleeping; }
    void setAllowSleeping(bool running);

    bool reportFps() const { return mReportFps; }
    void setReportFps(bool reportfps);

    float getFps() const { return mFps; }

    float getScaleRatio() const { return scaleRatio; }

    /**
     * The number of velocity iterations used to process one step.
     * 10 by default.
     */
    int velocityIterations() const
    { return mVelocityIterations; }

    void setVelocityIterations(int iterations)
    { mVelocityIterations = iterations; }


    /**
     * The number of position iterations used to process one step.
     * 10 by default.
     */
    int positionIterations() const
    { return mPositionIterations; }

    void setPositionIterations(int iterations)
    { mPositionIterations = iterations; }

    /**
     * The amount of time each frame takes in milliseconds.
     * By default it is 1000 / 60.
     */
    int frameTime() const { return mFrameTime; }
    void setFrameTime(int frameTime) { mFrameTime = frameTime; }

    QPointF gravity() const { return mGravity; }
    void setGravity(const QPointF &gravity);

    void componentComplete();

    void registerBody(Box2DBody *body);
    void registerJoint(Box2DJoint *joint);

    b2World *world() const { return mWorld; }

    Q_INVOKABLE void stop();
    Q_INVOKABLE void start();
    Q_INVOKABLE void resetVelocities();

private slots:
    void unregisterBody();
    void fixtureDestroyed(Box2DFixture *fixture);

signals:
    void gravityChanged();
    void runningChanged();
    void allowSleepingChanged();
    void stepped();
    void reportFpsChanged();
    void fpsChanged();

protected:
    void timerEvent(QTimerEvent *);
    QVariant itemChange(GraphicsItemChange, const QVariant &);

private:
    b2World *mWorld;
    ContactListener *mContactListener;
    Box2DDestructionListener *mDestructionListener;
    float mTimeStep;
    int mVelocityIterations;
    int mPositionIterations;
    int mFrameTime;
    QPointF mGravity;
    bool mIsRunning;
    bool mAllowSleeping;
    QBasicTimer mTimer;
    QList<Box2DBody*> mBodies;
    QTime time;
    int mFps;
    bool mReportFps;
    float lastTime;
    float mAc;
};

QML_DECLARE_TYPE(Box2DWorld)

#endif // BOX2DWORLD_H
