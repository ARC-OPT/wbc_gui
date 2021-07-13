#require 'vizkit'
require 'conversions'

module Wbc

    class PropertyInterface
        attr_reader :property_name
        attr_reader :interface_name
        attr_reader :to_raw_converter
        attr_reader :from_raw_converter
        def initialize(property_name, interface_name, to_raw_converter, from_raw_converter)
            @property_name = property_name
            @interface_name = interface_name
            @to_raw_converter = to_raw_converter
            @from_raw_converter = from_raw_converter
        end
    end

    class ControllerUI
        include Conversions

        def hidden?
            return @widget.isHidden == true
        end

        def showMessageBox(type, title, text)
            box = Qt::MessageBox.new
            box.icon = type
            box.text = text
            box.windowTitle = title
            box.show
        end

        def initialize(task, widget)
            @task = task
            @field_names = @task.field_names
            @proxy = task.to_proxy
            @task_name = task.name
            @widget = widget
            @data_listeners = Hash.new

            # Communicate task's state change and reachability to gui
            @proxy.on_reachable do
                 widget.setEnabled(true)
            end
            @proxy.on_unreachable do
                widget.checkKeepSending(false)
                widget.setEnabled(false)
            end
            @proxy.on_state_change do |state|
                widget.updateState(state.to_s)
            end

            # Apply all properties and reconfigure the connected task context
            @properties = []
            @widget.connect(SIGNAL('applyConfig()')) {
                @properties.each do |prop|
                    applyProperty prop.property_name, prop.from_raw_converter, @widget.query(String.new(prop.interface_name))
                end
                reconfigureTask @task_name
            }

            @widget.connect(SIGNAL('importConfig(QString, QString)')) do |file_name, section_name|
              splitted = section_name.split(",")
              begin
                @task.apply_conf_file(file_name, splitted, true)
                reconfigureTask @task_name
              rescue Exception => e
                showMessageBox Qt::MessageBox.Warning, "Import Error",
                               "Unable to apply config section(s) " + splitted.to_s + " from file " + file_name
              end
            end

            @widget.connect(SIGNAL('exportConfig(QString, QString)')) do |file_name, section_name|
              begin
                @task.save_conf(file_name, section_name)
              rescue Exception => e
                 showMessageBox Qt::MessageBox.Warning, "Import Error",
                                "Unable to save config to file " + file_name
              end
            end

            @widget.connect(SIGNAL('getCurrentPath(bool)')) do |isImportDialog|
              path = ENV['AUTOPROJ_CURRENT_ROOT']
              print path
              if isImportDialog
                @widget.openImportDialog(path.dup)
              else
                @widget.openExportDialog(path.dup)
              end
            end
        end

        def addPropertyInterface(prop_name, to_raw_converter, from_raw_converter, interface_name = prop_name)
            # Add property config widget to gui
            @widget.addConfigInterface(interface_name)
            # # Store converters for this property
            @properties << PropertyInterface.new(prop_name, interface_name, to_raw_converter, from_raw_converter)
            # Read initial value and update the corresponding property interface in GUI
            @proxy.property(prop_name).on_change do |value|
                updatePropertyInterface interface_name, to_raw_converter, value
            end
            # If reset is clicked, update the corresponding property interface in GUI
            @widget.connect(SIGNAL('reset()')) {
                @proxy.property(prop_name).read do |value|
                    updatePropertyInterface interface_name, to_raw_converter, value
                end
            }
        end

        def addOutputPortInterface(port_name, to_raw_converter)
            # Add port interface widget to gui
            @widget.addOutputPortInterface(port_name)

            @widget.connect(SIGNAL('showDetails()')) do
                # If there is new data on port, update the corresponding port interface in GUI
                @data_listeners[port_name] = @proxy.port(port_name).on_data do |data|
                    updatePortInterface port_name, to_raw_converter, data if !hidden?
                end
            end

            @widget.connect(SIGNAL('hideDetails()')) do
                @data_listeners[port_name].stop
                @data_listeners.delete port_name
            end
        end

        def addInputPortInterface(port_name, from_raw_converter)
            # Add port interface widget to gui
            @widget.addInputPortInterface(port_name)
            # Query data from GUI, convert to typed values and write to input port
            @widget.connect(SIGNAL('send()')) {
                writePort port_name, from_raw_converter, @widget.query(String.new(port_name))
            }
        end

        def updatePropertyInterface(interface_name, to_raw_converter, typed_values)
            raw_values = send(to_raw_converter, typed_values)
            if raw_values.size == 0 # Ugly fix for the case where on_change of a property is called and the property is unset
                return
            end
            @widget.updateData String.new(interface_name), raw_values
        end

        def applyProperty(prop_name, from_raw_converter, raw_values )
            typed_values = send(from_raw_converter, raw_values)
            @proxy.property(prop_name).write(typed_values) do end
        end

        def updatePortInterface(port_name, to_raw_converter, typed_values)
            raw_values = send(to_raw_converter, typed_values)
            if raw_values.size == 0 # Ugly fix for the case where on_change of a property is called and the property is unset
                return
            end
            @widget.updateData port_name, raw_values
        end

        def writePort(port_name, from_raw_converter, raw_values)
            typed_values = send(from_raw_converter, raw_values)
            @proxy.port(port_name).write(typed_values) do end
        end

        def reconfigureTask(task_name)
            @task = Orocos::name_service.get task_name
            has_been_running = false
            if @task.running?
                has_been_running = true
                @task.stop
            end
            if @task.state == :STOPPED
                @task.cleanup
            end
            @task.configure
            if has_been_running
                @task.start
            end
        end
    end
end
