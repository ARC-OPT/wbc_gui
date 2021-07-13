require "controller_ui"

module Wbc
    class JointLimitAvoidanceUI < ControllerUI
        def fromJointLimits(limits, field)
            arr = []
            limits.elements.each do |elem|
                arr << elem.get_field(field).position
            end
            return arr
        end

        def toJointLimits(upper, lower)
            limits = Types.base.JointLimits.new
            upper.zip(lower).each do |u, l|
                range = Types.base.JointLimitRange.new
                range.max.position = u
                range.min.position = l
                limits.elements << range
            end
            return limits
        end

        def updateLimits(widget, task_name)
            @task = Orocos::TaskContext.get task_name
            widget.updateData("Upper Limit", fromJointLimits(@task.joint_limits, "max"))
            widget.updateData("Lower Limit", fromJointLimits(@task.joint_limits, "min"))

            if @task.influence_distance_per_field.empty?
                widget.updateData("Influence Distance", [@task.influence_distance]*@field_names.size)
            else
                widget.updateData("Influence Distance", fromInfluenceDistance(@task.influence_distance_per_field))
            end
        end

        def writeLimits(widget, task_name)
            @proxy.property("joint_limits").write toJointLimits(widget.query("Upper Limit"), widget.query("Lower Limit")) do end
            @proxy.property("influence_distance_per_field").write toInfluenceDistance(widget.query("Influence Distance"), @field_names) do end
        end

        def initialize(task_name, widget)
            super

            addPropertyInterface("p_gain",             :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("influence_distance", :vectorXdToRaw, :rawToVectorXd)
            addPropertyInterface("max_control_output", :vectorXdToRaw, :rawToVectorXd)

            addOutputPortInterface("current_feedback",   :jointsToRawPositions)
            addOutputPortInterface("control_output",     :jointsToRawSpeeds)

            # Specializations for this type of controller:
            @widget.addOutputPortInterface("distance_to_limit")
            @widget.connect(SIGNAL('showDetails()')) do
                @data_listeners["field_infos"] = @proxy.port("field_infos").on_data do |field_infos|
                    arr = []
                    field_infos.each do |info|
                        arr << info.euclidean_distance
                    end
                    @widget.updateData("distance_to_limit", arr) if !hidden?
                end
            end

            @widget.connect(SIGNAL('hideDetails()')) do
                @data_listeners["field_infos"].stop
                @data_listeners.delete "field_infos"
            end
        end
    end # class
end # module
