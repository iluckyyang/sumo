/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    MSPhaseDefinition.h
/// @author  Daniel Krajzewicz
/// @author  Julia Ringel
/// @author  Jakob Erdmann
/// @author  Sascha Krieg
/// @author  Michael Behrisch
/// @date    Jan 2004
/// @version $Id$
///
// The definition of a single phase of a tls logic
/****************************************************************************/
#ifndef MSPhaseDefinition_h
#define MSPhaseDefinition_h

#define TARGET_BIT 0
#define TRANSIENT_NOTDECISIONAL_BIT 1
#define COMMIT_BIT 2
#define UNDEFINED_BIT 3



// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include <bitset>
#include <string>
#include <vector>
#include <utils/common/MsgHandler.h>
#include <utils/common/SUMOTime.h>
#include <utils/options/OptionsCont.h>
#include <utils/xml/SUMOXMLDefinitions.h>


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class MSPhaseDefinition
 * @brief The definition of a single phase of a tls logic
 */
class MSPhaseDefinition {
public:
    /*
     * @brief The definition of phase types
     * Phase types are compulsory directives for SOTL policies.
     * Knowing the phase type a policy can drive complex junction that need higly customized phases.
     * Leaving the phase type as "undefined" makes SOTL policies to malfunction.
     * Four bits:
     * TARGET_BIT 0 -> the phase is a target one
     * TRANSIENT_NOTDECISIONAL_BIT 1 -> the phase is a transient one or a decisional one
     * COMMIT_BIT 2 -> the phase is a commit one
     * UNDEFINED_BIT 3 -> the phase type is undefined
     */
    typedef std::bitset<4> PhaseType;

    typedef std::vector<std::string> LaneIdVector;

public:
    /// @brief The duration of the phase
    SUMOTime duration;

    /// @brief The previous duration of the phase
    SUMOTime lastDuration;

    /// @brief The minimum duration of the phase
    SUMOTime minDuration;

    /// @brief The maximum duration of the phase
    SUMOTime maxDuration;

    /// @brief Stores the timestep of the last on-switched of the phase
    SUMOTime myLastSwitch;

    /// @brief The index of the phase that suceeds this one (or -1)
    int nextPhase;

private:
    /// @brief The phase definition
    std::string state;

    /*
    * The type of this phase
    */
    PhaseType phaseType;

    /*
     * @brief The lanes-set
     * This array can be null if this phase is not a target step,
     * otherwise, a bit is true if the corresponding lane belongs to a
     * set of input lanes.
     * SOTL traffic light logics choose the target step according to sensors
     * belonging to the lane-set.
     */
    LaneIdVector targetLaneSet;

    void init(SUMOTime durationArg, const std::string& stateArg, SUMOTime minDurationArg, SUMOTime maxDurationArg, int nextPhase) {
        this->duration = durationArg;
        this->state = stateArg;
        this->minDuration = minDurationArg < 0 ? durationArg : minDurationArg;
        this->maxDuration = (maxDurationArg < 0 || maxDurationArg < minDurationArg) ? durationArg : maxDurationArg;
        // assert(this->minDuration <= this->maxDuration); // not ensured by the previous lines
        this->myLastSwitch = string2time(OptionsCont::getOptions().getString("begin")); // SUMOTime-option
        //For SOTL phases
        //this->phaseType = phaseTypeArg;
        this->nextPhase = nextPhase;
    }

    void init(SUMOTime durationArg, SUMOTime minDurationArg, SUMOTime maxDurationArg, const std::string& stateArg, int nextPhase, LaneIdVector& targetLaneSetArg) {
        init(durationArg, stateArg, minDurationArg, maxDurationArg, nextPhase);
        //For SOTL target phases
        this->targetLaneSet = targetLaneSetArg;
    }


public:
    /** @brief Constructor
     *
     * minDuration and maxDuration are set to duration.
     *
     * @param[in] durationArg The duration of the phase
     * @param[in] stateArg The state in the phase
     */
    MSPhaseDefinition(SUMOTime durationArg, const std::string& stateArg, int nextPhase) {
        //PhaseType phaseType;
        phaseType = PhaseType();
        phaseType[UNDEFINED_BIT] = 1;
        phaseType[TRANSIENT_NOTDECISIONAL_BIT] = 0;
        phaseType[TARGET_BIT] = 0;
        phaseType[COMMIT_BIT] = 0;
        init(durationArg, stateArg, durationArg, durationArg, nextPhase);
    }


    /** @brief Constructor
     * In this phase the duration is constrained between min and max duration
     * @param[in] durationArg The duration of the phase
     * @param[in] stateArg The state in the phase
     * @param[in] minDurationArg The minimum duration of the phase
     * @param[in] maxDurationArg The maximum duration of the phase
     */
    MSPhaseDefinition(SUMOTime durationArg, const std::string& stateArg, SUMOTime minDurationArg=-1, SUMOTime maxDurationArg=-1, int nextPhase=-1) {
        //PhaseType phaseType;
        phaseType = PhaseType();
        phaseType[UNDEFINED_BIT] = 1;
        phaseType[TRANSIENT_NOTDECISIONAL_BIT] = 0;
        phaseType[TARGET_BIT] = 0;
        phaseType[COMMIT_BIT] = 0;
        init(durationArg, stateArg, minDurationArg, maxDurationArg, nextPhase);
    }

    /*
     * @brief Constructor for definitions for SOTL target step
     * In this phase the duration is fixed, because min and max duration are unspecified
     * @param[in] transient_notdecisional true means this is a transient phase, false is a decisional one
     * @param[in] commit true means this is a commit phase, i.e. the traffic light logic can jump to a target phase form this phase
     * @param[in] targetLaneSet identifies the lane-set to be considered when deciding the target phase to jump to
     * @see MSPhaseDefinition::PhaseType
     */
    MSPhaseDefinition(SUMOTime durationArg, const std::string& stateArg, int nextPhase,  bool transient_notdecisional, bool commit, LaneIdVector& targetLaneSetArg) {
        if (targetLaneSetArg.size() == 0) {
            MsgHandler::getErrorInstance()->inform("MSPhaseDefinition::MSPhaseDefinition -> targetLaneSetArg cannot be empty for a target phase");
        }
        //PhaseType phaseType;
        phaseType = PhaseType();
        phaseType[UNDEFINED_BIT] = 0;
        phaseType[TRANSIENT_NOTDECISIONAL_BIT] = transient_notdecisional;
        phaseType[TARGET_BIT] = 1;
        phaseType[COMMIT_BIT] = commit;
        init(durationArg, durationArg, durationArg, stateArg, nextPhase, targetLaneSetArg);
    }

    /*
     * @brief Constructor for definitions for SOTL non-target step
     * In this phase the duration is fixed, because min and max duration are unspecified
     * @param[in] phaseType Indicates the type of the step
     */
    MSPhaseDefinition(SUMOTime durationArg, const std::string& stateArg, int nextPhase, bool transient_notdecisional, bool commit) {
        //PhaseType phaseType;
        phaseType = PhaseType();
        phaseType[UNDEFINED_BIT] = 0;
        phaseType[TRANSIENT_NOTDECISIONAL_BIT] = transient_notdecisional;
        phaseType[TARGET_BIT] = 0;
        phaseType[COMMIT_BIT] = commit;
        init(durationArg, stateArg, durationArg, durationArg, nextPhase);
    }


    /*
     * @brief Constructor for definitions for SOTL target step
     * In this phase the duration is constrained between min and max duration
     * @param[in] phaseType Indicates the type of the step
     * @param[in] targetLaneSet If not null, specifies this MSPhaseDefinition is a target step
     * @see MSPhaseDefinition::PhaseType
     */
    MSPhaseDefinition(SUMOTime durationArg, const std::string& stateArg, SUMOTime minDurationArg, SUMOTime maxDurationArg, int nextPhase, bool transient_notdecisional, bool commit, LaneIdVector& targetLaneSetArg) {
        if (targetLaneSetArg.size() == 0) {
            MsgHandler::getErrorInstance()->inform("MSPhaseDefinition::MSPhaseDefinition -> targetLaneSetArg cannot be empty for a target phase");
        }
        //PhaseType phaseType;
        //phaseType = PhaseType::bitset();
        phaseType = PhaseType();
        phaseType[UNDEFINED_BIT] = 0;
        phaseType[TRANSIENT_NOTDECISIONAL_BIT] = transient_notdecisional;
        phaseType[TARGET_BIT] = 1;
        phaseType[COMMIT_BIT] = commit;
        init(durationArg, minDurationArg, maxDurationArg, stateArg, nextPhase, targetLaneSetArg);
    }

    /*
     * @brief Constructor for definitions for SOTL target step
     * In this phase the duration is constrained between min and max duration
     * @param[in] phaseType Indicates the type of the step
     * @see MSPhaseDefinition::PhaseType
     */
    MSPhaseDefinition(SUMOTime durationArg, const std::string& stateArg, SUMOTime minDurationArg, SUMOTime maxDurationArg, int nextPhase, bool transient_notdecisional, bool commit) {
        //PhaseType phaseType;
        phaseType = PhaseType();
        phaseType[UNDEFINED_BIT] = 0;
        phaseType[TRANSIENT_NOTDECISIONAL_BIT] = transient_notdecisional;
        phaseType[TARGET_BIT] = 0;
        phaseType[COMMIT_BIT] = commit;
        init(durationArg, stateArg, minDurationArg, maxDurationArg, nextPhase);

    }


    /// @brief Destructor
    virtual ~MSPhaseDefinition() { }


    /** @brief Returns the state within this phase
     * @return The state in this phase
     */
    const std::string& getState() const {
        return state;
    }

    void setState(const std::string& _state) {
        state = _state;
    }

    const LaneIdVector& getTargetLaneSet() const {
        return targetLaneSet;
    }

    int getNextPhase() const {
        return nextPhase;
    }

    /** @brief Returns whether this phase is a pure "green" phase
     *
     * "pure green" means in this case that at least one stream has green
     *  and no stream has yellow. Such phases are meant to be candidates
     *  for being stretched by actuated or agentbased traffic light logics.
     * @return Whether this phase is a "pure green" phase
     */
    bool isGreenPhase() const {
        if (state.find_first_of("gG") == std::string::npos) {
            return false;
        }
        if (state.find_first_of("yY") != std::string::npos) {
            return false;
        }
        return true;
    }


    /** @brief Returns the state of the tls signal at the given position
     * @param[in] pos The position of the signal to return the state for
     * @return The state of the signal at the given position
     */
    LinkState getSignalState(int pos) const {
        return (LinkState) state[pos];
    }


    /** @brief Comparison operator
     *
     * Note that only the state must differ, not the duration!
     * @param[in] pd The phase definition to compare against
     * @return Whether the given phase definition differs
     */
    bool operator!=(const MSPhaseDefinition& pd) {
        return state != pd.state;
    }


    /*
    * @return true if the phase type is undefined
    */
    bool isUndefined() const {
        return phaseType[UNDEFINED_BIT];
    }

    /*
    * @return true if this is a target phase
    */
    bool isTarget() const {
        return phaseType[TARGET_BIT];
    }

    /*
    * @return true if this is a transient phase
    */
    bool isTransient() const {
        return phaseType[TRANSIENT_NOTDECISIONAL_BIT];
    }

    /*
    * @return true if this is a decisional phase
    */
    bool isDecisional() const {
        return !phaseType[TRANSIENT_NOTDECISIONAL_BIT];
    }

    /*
    * @return true if this is a commit phase
    */
    bool isCommit() const {
        return phaseType[COMMIT_BIT];
    }

};

#endif

/****************************************************************************/

