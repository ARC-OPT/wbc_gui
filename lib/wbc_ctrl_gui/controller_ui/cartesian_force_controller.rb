require "controller_ui"

module Wbc
    class CartesianForceControllerUI < ControllerUI
        def initialize(task_name, widget)
            super
            addPropertyInterface("p_gain",             :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("max_control_output", :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("dead_zone",          :vectorXdToRaw, :rawToVectorXd)

            addInputPortInterface("setpoint",          :rawToWrench)
            addOutputPortInterface("control_output",   :RigidBodyStateSE3ToRawTwist)
            addOutputPortInterface("current_setpoint", :wrenchToRaw)
            addOutputPortInterface("current_feedback", :wrenchToRaw)

            # Specializations for this type of controller: Initialize the setpoint fields (in the GUI)
            # with the current feedback in the beginning and if reset is clicked
            @proxy.port("current_feedback").once_on_data do |data|
                updatePortInterface("setpoint", :wrenchToRaw, data)
            end
            @widget.connect(SIGNAL('reset()')) {
                sample = @proxy.port("current_feedback").last_sample
                updatePortInterface("setpoint", :wrenchToRaw, sample) if sample
            }
        end
    end # class
end # module
