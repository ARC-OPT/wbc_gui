require "controller_ui"

module Wbc
    class CartesianRadialPotentialFieldsUI < ControllerUI
        def initialize(task_name, widget)
            super

            addPropertyInterface("p_gain",             :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("max_control_output", :vectorXdToRaw, :rawToVectorXd)

            addOutputPortInterface("control_output",   :RigidBodyStateSE3ToRawSpeed)
            addOutputPortInterface("current_feedback", :RigidBodyStateSE3ToRawPosition)
        end
    end # class
end # module
