/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2007-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    MSPersonDevice_Routing.h
/// @author  Michael Behrisch
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @date    Tue, 04 Dec 2007
/// @version $Id$
///
// A device that performs vehicle rerouting based on current edge speeds
/****************************************************************************/
#ifndef MSPersonDevice_Routing_h
#define MSPersonDevice_Routing_h


// ===========================================================================
// included modules
// ===========================================================================
#include <config.h>

#include "MSPersonDevice.h"


// ===========================================================================
// class declarations
// ===========================================================================
class MSLane;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class MSPersonDevice_Routing
 * @brief A device that performs person rerouting based on current edge speeds
 *
 * The routing-device system consists of in-vehicle devices that perform the routing
 *  and simulation-wide static methods for collecting edge weights and
 *  parallelizing in MSRoutingEngine.
 *
 * A device is assigned using the common explicit/probability - procedure.
 *
 * A device computes a new route for a person as soon as a personTrip appears in the plan.
 */
class MSPersonDevice_Routing : public MSPersonDevice {
public:
    /** @brief Inserts MSPersonDevice_Routing-options
     * @param[filled] oc The options container to add the options to
     */
    static void insertOptions(OptionsCont& oc);

    /** @brief checks MSPersonDevice_Routing-options
     * @param[filled] oc The options container with the user-defined options
     */
    static bool checkOptions(OptionsCont& oc);


    /** @brief Build devices for the given person, if needed
     *
     * The options are read and evaluated whether rerouting-devices shall be built
     *  for the given person.
     *
     * When the first device is built, the static container of edge weights
     *  used for routing is initialised with the mean speed the edges allow.
     *  In addition, an event is generated which updates these weights is
     *  built and added to the list of events to execute at a simulation end.
     *
     * The built device is stored in the given vector.
     *
     * @param[in] p The person for which a device may be built
     * @param[filled] into The vector to store the built device in
     */
    static void buildDevices(MSTransportable& p, std::vector<MSPersonDevice*>& into);


    /// @brief Destructor.
    ~MSPersonDevice_Routing();


    /// @brief return the name for this type of device
    const std::string deviceName() const {
        return "rerouting";
    }

    /** @brief Saves the state of the device
     *
     * @param[in] out The OutputDevice to write the information into
     */
    void saveState(OutputDevice& out) const;

    /** @brief Loads the state of the device from the given description
     *
     * @param[in] attrs XML attributes describing the current state
     */
    void loadState(const SUMOSAXAttributes& attrs);

    /// @brief initiate the rerouting, create router / thread pool on first use
    void reroute(const SUMOTime currentTime, const bool onInit = false);

    /// @brief try to retrieve the given parameter from this device. Throw exception for unsupported key
    std::string getParameter(const std::string& key) const;

    /// @brief try to set the given parameter for this device. Throw exception for unsupported key
    void setParameter(const std::string& key, const std::string& value);


private:

    /** @brief Constructor
     *
     * @param[in] holder The vehicle that holds this device
     * @param[in] id The ID of the device
     * @param[in] period The period with which a new route shall be searched
     */
    MSPersonDevice_Routing(MSTransportable& holder, const std::string& id, SUMOTime period);

    /** @brief Performs rerouting after a period
     *
     * A new route is computed by calling the vehicle's "reroute" method, supplying
     *  "getEffort" as the edge effort retrieval method.
     *
     * This method is called from the event handler at the begin of a simulation
     *  step after the rerouting period is over. The reroute period is returned.
     *
     * @param[in] currentTime The current simulation time
     * @return The offset to the next call (the rerouting period "myPeriod")
     * @see MSVehicle::reroute
     * @see MSEventHandler
     * @see WrappingCommand
     */
    SUMOTime wrappedRerouteCommandExecute(SUMOTime currentTime);


private:
    /// @brief The period with which a vehicle shall be rerouted
    SUMOTime myPeriod;

    /// @brief The last time a routing took place
    SUMOTime myLastRouting;

    /// @brief The (optional) command responsible for rerouting
    WrappingCommand< MSPersonDevice_Routing >* myRerouteCommand;

private:
    /// @brief Invalidated copy constructor.
    MSPersonDevice_Routing(const MSPersonDevice_Routing&);

    /// @brief Invalidated assignment operator.
    MSPersonDevice_Routing& operator=(const MSPersonDevice_Routing&);


};


#endif

/****************************************************************************/
