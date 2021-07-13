require "controller_ui"

module Wbc
    class JointPositionControllerUI < ControllerUI
        def rawPositionsToJointsWithNames(positions)
            rawPositionsToJoints positions, @field_names
        end

        def initialize(task_name, widget)
            super
            addPropertyInterface("p_gain",             :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("d_gain",             :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("max_control_output", :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("dead_zone",          :vectorXdToRaw, :rawToVectorXd)

            # Add interfaces
            addInputPortInterface("setpoint",          :rawPositionsToJointsWithNames)
            addOutputPortInterface("control_output",   :jointsToRawSpeeds)
            addOutputPortInterface("current_setpoint", :jointsToRawPositions)
            addOutputPortInterface("current_feedback", :jointsToRawPositions)

            # Specializations for this type of controller: Initialize the setpoint fields (in the GUI)
            # with the current feedback in the beginning and if reset is clicked
            @proxy.port("current_feedback").once_on_data do |data|
                updatePortInterface("setpoint", :jointsToRawPositions, data)
            end
            @widget.connect(SIGNAL('reset()')) {
                sample = @proxy.port("current_feedback").last_sample
                updatePortInterface("setpoint", :jointsToRawPositions, sample) if sample
            }
        end
    end # class
end # module
