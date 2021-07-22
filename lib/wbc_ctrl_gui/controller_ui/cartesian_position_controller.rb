require "controller_ui"

module Wbc
    class CartesianPositionControllerUI < ControllerUI
        def initialize(task_name, widget)
            super
            addPropertyInterface("p_gain",             :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("d_gain",             :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("ff_gain",            :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("max_control_output", :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("dead_zone",          :vectorXdToRaw, :rawToVectorXd)

            addInputPortInterface("setpoint",          :rawPoseToRigidBodyStateSE3)
            addOutputPortInterface("control_output",   :RigidBodyStateSE3ToRawTwist)
            addOutputPortInterface("current_setpoint", :RigidBodyStateSE3ToRawPose)
            addOutputPortInterface("current_feedback", :RigidBodyStateSE3ToRawPose)

            # Specializations for this type of controller: Initialize the setpoint fields (in the GUI)
            # with the current feedback in the beginning and if reset is clicked
            @proxy.port("current_feedback").once_on_data do |data|
                updatePortInterface("setpoint", :RigidBodyStateSE3ToRawPose, data)
            end
            @widget.connect(SIGNAL('reset()')) {
                sample = @proxy.port("current_feedback").last_sample
                updatePortInterface("setpoint", :RigidBodyStateSE3ToRawPose, sample) if sample
            }
        end
    end # class
end # module
