require "vizkit"
Dir[File.expand_path(File.dirname(__FILE__)) + "/controller_ui/*.rb"].each {|file| require file }

module Wbc
    class WbcCtrlGUI

        attr_reader :widget

        # Init gui: Load widget and start pinging for tasks that contain the given search string
        #
        def initialize(ping_interval_sec = 5)
            @connections = Hash.new
            @widget = Vizkit.default_loader.WbcCtrlGui
            updateTasks("ctrl_lib::")
            startTaskPing("ctrl_lib::", ping_interval_sec)
        end

        # Connect all new tasks whose model name includes the given search string (e.g. ctrl_lib::) to the GUI
        #
        def updateTasks(model_search_string)
            names = Orocos::name_service.names
            names = names.sort
            names.each do |name|
                if not @connections.has_key? name
                    begin
                        task = Orocos::name_service.get name
                    rescue # Ignore if task context can not be retrieved from name server
                        next
                    end
                    if task.reachable? && task.model.name && task.model.name.include?(model_search_string)
                        @connections[name] = connectControllerUI(task)
                    end
                end
            end
        end

        # Connect the given controller to the corresponding widget
        #
        def connectControllerUI(task)
            connection_name = "Wbc::" + task.model.name + "UI"
            connection_name.slice! "ctrl_lib::"

            if !Object.const_defined?(connection_name)
                puts "No connection model for  " + task.name + "! Ignoring..."
                return nil
            end

            if task.field_names == nil
                puts "Ignoring task context " + task.name + " because it does not have a 'field_names' property"
                return
            end
            if task.field_names.size() == 0
                puts "Ignoring task context " + task.name + " because it's 'field_names' property is empty"
                return
            end

            # Add controller interface widget to a common gui
            @widget.addControllerInterface(task.name, task.field_names)
            # Create a new connection. Naming convention for connection class: If task model is called ctrl_lib::TaskXY,
            # the class has to be called TaskXYConnection and lie in namespace Wbc
            connection = Object.const_get(connection_name).new(task, @widget.getControllerInterface(task.name))

            return connection
        end

        # Start timer to update tasks with the given search string every <interval> ms
        #
        def startTaskPing(model_search_string, ping_interval_sec)
            @timer = Qt::Timer.new
            @timer.connect(SIGNAL('timeout()')) do
                updateTasks(model_search_string)
            end
            @timer.start(ping_interval_sec*1000)
        end
    end

end
