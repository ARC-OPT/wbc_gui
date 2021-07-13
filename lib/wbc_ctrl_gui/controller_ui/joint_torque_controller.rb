require "controller_ui"

module Wbc
    class JointTorqueControllerUI < ControllerUI
        def rawEffortsToJointsWithNames(efforts)
            rawEffortsToJoints efforts, @field_names
        end

        def rawGainsToPIDCtrlParams(gains)
            pid_params = Types.ctrl_lib.PIDCtrlParams.new
            pid_params.p_gain.from_a @widget.query("p_gain")
            pid_params.i_gain.from_a @widget.query("i_gain")
            pid_params.d_gain.from_a @widget.query("d_gain")
            pid_params.windup.from_a @widget.query("windup")
            return pid_params
        end

        def pidCtrlParamsPGainToRaw(pid_params)
            return pid_params.p_gain.to_a
        end

        def pidCtrlParamsIGainToRaw(pid_params)
            return pid_params.i_gain.to_a
        end

        def pidCtrlParamsDGainToRaw(pid_params)
            return pid_params.d_gain.to_a
        end

        def pidCtrlParamsWindupToRaw(pid_params)
            return pid_params.windup.to_a
        end

        def initialize(task_name, widget)
            super
            addPropertyInterface("pid_params",         :pidCtrlParamsPGainToRaw,  :rawGainsToPIDCtrlParams, "p_gain")
            addPropertyInterface("pid_params",         :pidCtrlParamsIGainToRaw,  :rawGainsToPIDCtrlParams, "i_gain")
            addPropertyInterface("pid_params",         :pidCtrlParamsDGainToRaw,  :rawGainsToPIDCtrlParams, "d_gain")
            addPropertyInterface("pid_params",         :pidCtrlParamsWindupToRaw, :rawGainsToPIDCtrlParams, "windup")
            addPropertyInterface("max_control_output", :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("dead_zone",          :vectorXdToRaw, :rawToVectorXd)

            # Add interfaces
            addInputPortInterface("setpoint",          :rawEffortsToJointsWithNames)
            addOutputPortInterface("control_output",   :jointsToRawSpeeds)
            addOutputPortInterface("current_setpoint", :jointsToRawEfforts)
            addOutputPortInterface("current_feedback", :jointsToRawEfforts)

            # Specializations for this type of controller: Initialize the setpoint fields (in the GUI)
            # with the current feedback in the beginning and if reset is clicked
            @proxy.port("current_feedback").once_on_data do |data|
                updatePortInterface("setpoint", :jointsToRawEfforts, data)
            end
            @widget.connect(SIGNAL('reset()')) {
                sample = @proxy.port("current_feedback").last_sample
                updatePortInterface("setpoint", :jointsToRawEfforts, sample) if sample
            }
        end
    end # class
end # module
