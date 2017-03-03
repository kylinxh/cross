#ifndef __ACTIONS_CCACTION_H__
#define __ACTIONS_CCACTION_H__

#include "basics/CAObject.h"
#include "ccTypes.h"
#include "script_support/CCScriptSupport.h"
NS_CC_BEGIN

class CGNode;

enum {
    kActionUpdate
};

/**
 * @addtogroup actions
 * @{
 */

/** 
 * @brief Base class for Action objects.
 */
class CC_DLL Action : public CAObject, public CAClonable
{
public:
    /** Default tag used for all the actions. */
    static const int INVALID_TAG = -1;
    /**
     * @js NA
     * @lua NA
     */
    virtual std::string description() const;

    /** Returns a clone of action.
     *
     * @return A clone action.
     */
    virtual Action* clone() const
    {
        CC_ASSERT(0);
        return nullptr;
    }

    /** Returns a new action that performs the exact reverse of the action. 
     *
     * @return A new action that performs the exact reverse of the action.
     * @js NA
     */
    virtual Action* reverse() const
    {
        CC_ASSERT(0);
        return nullptr;
    }

    /** Return true if the action has finished. 
     * 
     * @return Is true if the action has finished.
     */
    virtual bool isDone() const;

    /** Called before the action start. It will also set the target. 
     *
     * @param target A certain target.
     */
    virtual void startWithTarget(CGNode *target);

    /** 
     * Called after the action has finished. It will set the 'target' to nil.
     * IMPORTANT: You should never call "Action::stop()" manually. Instead, use: "target->stopAction(action);".
     */
    virtual void stop();

    /** Called every frame with it's delta time, dt in seconds. DON'T override unless you know what you are doing. 
     *
     * @param dt In seconds.
     */
    virtual void step(float dt);

    /** 
     * Called once per frame. time a value between 0 and 1.

     * For example:
     * - 0 Means that the action just started.
     * - 0.5 Means that the action is in the middle.
     * - 1 Means that the action is over.
     *
     * @param time A value between 0 and 1.
     */
    virtual void update(float time);
    /** Return certain target.
     *
     * @return A certain target.
     */
    inline CGNode* getTarget() const { return _target; }
    /** The action will modify the target properties. 
     *
     * @param target A certain target.
     */
    inline void setTarget(CGNode *target) { _target = target; }
    /** Return a original Target. 
     *
     * @return A original Target.
     */
    inline CGNode* getOriginalTarget() const { return _originalTarget; }
    /** 
     * Set the original target, since target can be nil.
     * Is the target that were used to run the action. Unless you are doing something complex, like ActionManager, you should NOT call this method.
     * The target is 'assigned', it is not 'retained'.
     * @since v0.8.2
     *
     * @param originalTarget Is 'assigned', it is not 'retained'.
     */
    inline void setOriginalTarget(CGNode *originalTarget) { _originalTarget = originalTarget; }
    /** Returns a tag that is used to identify the action easily. 
     *
     * @return A tag.
     */
    inline int getTag() const { return _tag; }
    /** Changes the tag that is used to identify the action easily. 
     *
     * @param tag Used to identify the action easily.
     */
    inline void setTag(int tag) { _tag = tag; }
    /** Returns a flag field that is used to group the actions easily.
     *
     * @return A tag.
     */
    inline unsigned int getFlags() const { return _flags; }
    /** Changes the flag field that is used to group the actions easily.
     *
     * @param tag Used to identify the action easily.
     */
    inline void setFlags(unsigned int flags) { _flags = flags; }

public:
    Action();
    virtual ~Action();

protected:
    CGNode    *_originalTarget;
    /** 
     * The "target".
     * The target will be set with the 'startWithTarget' method.
     * When the 'stop' method is called, target will be set to nil.
     * The target is 'assigned', it is not 'retained'.
     */
    CGNode    *_target;
    /** The action tag. An identifier of the action. */
    int     _tag;
    /** The action flag field. To categorize action into certain groups.*/
    unsigned int _flags;

#if CC_ENABLE_SCRIPT_BINDING
    ccScriptType _scriptType;         ///< type of script binding, lua or javascript
#endif
private:
    M_DISALLOW_COPY_AND_ASSIGN(Action);
};

/** @class FiniteTimeAction
 * @brief
 * Base class actions that do have a finite time duration.
 * Possible actions:
 * - An action with a duration of 0 seconds.
 * - An action with a duration of 35.5 seconds.
 * Infinite time actions are valid.
 */
class CC_DLL FiniteTimeAction : public Action
{
public:
    /** Get duration in seconds of the action. 
     *
     * @return The duration in seconds of the action.
     */
    inline float getDuration() const { return _duration; }
    /** Set duration in seconds of the action. 
     *
     * @param duration In seconds of the action.
     */
    inline void setDuration(float duration) { _duration = duration; }

    //
    // Overrides
    //
    virtual FiniteTimeAction* reverse() const override
    {
        CC_ASSERT(0);
        return nullptr;
    }
    virtual FiniteTimeAction* clone() const override
    {
        CC_ASSERT(0);
        return nullptr;
    }

public:
    FiniteTimeAction()
    : _duration(0)
    {}
    virtual ~FiniteTimeAction(){}

protected:
    //! Duration in seconds.
    float _duration;
private:
    M_DISALLOW_COPY_AND_ASSIGN(FiniteTimeAction);
};

class ActionInterval;
class RepeatForever;

/** @class Speed
 * @brief Changes the speed of an action, making it take longer (speed>1)
 * or shorter (speed<1) time.
 * Useful to simulate 'slow motion' or 'fast forward' effect.
 * @warning This action can't be Sequenceable because it is not an IntervalAction.
 */
class CC_DLL Speed : public Action
{
public:
    /** Create the action and set the speed.
     *
     * @param action An action.
     * @param speed The action speed.
     */
    static Speed* create(ActionInterval* action, float speed);
    /** Return the speed.
     *
     * @return The action speed.
     */
    inline float getSpeed(void) const { return _speed; }
    /** Alter the speed of the inner function in runtime. 
     *
     * @param speed Alter the speed of the inner function in runtime.
     */
    inline void setSpeed(float speed) { _speed = speed; }

    /** Replace the interior action.
     *
     * @param action The new action, it will replace the running action.
     */
    void setInnerAction(ActionInterval *action);
    /** Return the interior action.
     *
     * @return The interior action.
     */
    inline ActionInterval* getInnerAction() const { return _innerAction; }

    //
    // Override
    //
    virtual Speed* clone() const override;
    virtual Speed* reverse() const override;
    virtual void startWithTarget(CGNode* target) override;
    virtual void stop() override;
    /**
     * @param dt in seconds.
     */
    virtual void step(float dt) override;
    /** Return true if the action has finished.
     *
     * @return Is true if the action has finished.
     */
    virtual bool isDone() const  override;
    
public:
    Speed();
    virtual ~Speed(void);
    /** Initializes the action. */
    bool initWithAction(ActionInterval *action, float speed);

protected:
    float _speed;
    ActionInterval *_innerAction;

private:
    M_DISALLOW_COPY_AND_ASSIGN(Speed);
};

/** @class Follow
 * @brief Follow is an action that "follows" a CGNode.
 * Eg:
 * @code
 * layer->runAction(Follow::create(hero));
 * @endcode
 * Instead of using Camera as a "follower", use this action instead.
 * @since v0.99.2
 */
class CC_DLL Follow : public Action
{
public:
    /**
     * Creates the action with a set boundary or with no boundary.
     *
     * @param followedNode  The CGNode to be followed.
     * @param rect  The boundary. If \p rect is equal to DRectZero, it'll work
     *              with no boundary.
     */
    static Follow* create(CGNode *followedNode, const DRect& rect = DRectZero);
    /** Return boundarySet.
     *
     * @return Return boundarySet.
     */
    inline bool isBoundarySet() const { return _boundarySet; }
    /** Alter behavior - turn on/off boundary. 
     *
     * @param value Turn on/off boundary.
     */
    inline void setBoundarySet(bool value) { _boundarySet = value; }
    
    /** @deprecated Alter behavior - turn on/off boundary. 
     *
     * @param value Turn on/off boundary.
     */
    CC_DEPRECATED_ATTRIBUTE inline void setBoudarySet(bool value) { setBoundarySet(value); }

    //
    // Override
    //
    virtual Follow* clone() const override;
    virtual Follow* reverse() const override;
    /**
     * @param dt in seconds.
     * @js NA
     */
    virtual void step(float dt) override;
    virtual bool isDone() const override;
    virtual void stop() override;

public:
    /**
     * @js ctor
     */
    Follow()
    : _followedNode(nullptr)
    , _boundarySet(false)
    , _boundaryFullyCovered(false)
    , _leftBoundary(0.0)
    , _rightBoundary(0.0)
    , _topBoundary(0.0)
    , _bottomBoundary(0.0)
    , _worldRect(DRectZero)
    {}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~Follow();
    
    /**
     * Initializes the action with a set boundary or with no boundary.
     *
     * @param followedNode  The CGNode to be followed.
     * @param rect  The boundary. If \p rect is equal to DRectZero, it'll work
     *              with no boundary.
     */
    bool initWithTarget(CGNode *followedNode, const DRect& rect = DRectZero);

protected:
    /** CGNode to follow. */
    CGNode *_followedNode;

    /** Whether camera should be limited to certain area. */
    bool _boundarySet;

    /** If screen size is bigger than the boundary - update not needed. */
    bool _boundaryFullyCovered;

    /** Fast access to the screen dimensions. */
    DPoint _halfScreenSize;
    DPoint _fullScreenSize;

    /** World boundaries. */
    float _leftBoundary;
    float _rightBoundary;
    float _topBoundary;
    float _bottomBoundary;
    DRect _worldRect;

private:
    M_DISALLOW_COPY_AND_ASSIGN(Follow);
};

// end of actions group
/// @}

NS_CC_END

#endif // __ACTIONS_CCACTION_H__
