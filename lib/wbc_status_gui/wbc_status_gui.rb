module Wbc
    class WbcStatusGUI
	    attr_reader :widget

        def connectSolverUI()
            # Solver output
            @wbc_proxy.port("solver_output").on_data do |data|
                @widget.updateCtrlOut(data)
            end
            # Joint weights
            @wbc_proxy.port("current_joint_weights").on_data do |data|
                @widget.updateJointWeights(data)
            end
            @widget.connect(SIGNAL('jointWeightsChanged()')) { @wbc_proxy.port("joint_weights").write(@widget.getJointWeights()) do end }
        end

        def connectTaskUI(wbc_config)
            # Connect Activation ports
            wbc_config.each do |config|
                task_widget = @widget.getTaskInterface(config.name)
                @activation_ports[config.name] = @wbc_proxy.port("activation_" + config.name)
                @task_weight_ports[config.name] = @wbc_proxy.port("weight_" + config.name)
                task_widget.connect(SIGNAL('activate()'))   {
                    @activation_ports[config.name].write(1) do end
                }
                task_widget.connect(SIGNAL('deactivate()')) {
                    @activation_ports[config.name].write(0) do end
                }

                # Connect weight ports
                task_widget.connect(SIGNAL('taskWeightsChanged()')) {
                    weight_vector = Types.base.VectorXd.from_a task_widget.getTaskWeights()
                    @wbc_proxy.port("weight_" + config.name).write(weight_vector) do end
                }

                task_widget.connect(SIGNAL('priorityChanged(int)')) do |value|
                    reconfigureWBC
                end

                @wbc_proxy.port("task_" + config.name).on_data do |data|
                    @widget.updateTask(data)
                end
            end
        end

        def disconnectUI()
            @wbc_task.wbc_config.each do |cfg|
                task_widget = @widget.getTaskInterface(config.name)
                task_widget.disconnect(SIGNAL('taskWeightsChanged()')) do end
                task_widget.disconnect(SIGNAL('activate()')) do end
                task_widget.disconnect(SIGNAL('deactivate()')) do end
            end
            @task_weight_ports.clear
            @activation_ports.clear
            @task_ports.clear
            @widget.disconnect(SIGNAL('jointWeightsChanged()')) do end
            @widget.disconnect(SIGNAL('reconfigureWBC()')) do end
        end

        def initialize()
            while not (@wbc_task)
                begin
                    @wbc_task = Orocos::name_service.get_provides "wbc::WbcTask"
                rescue
                    puts "Waiting for WBC ..."
                    sleep 1
                end
            end

            @task_weight_ports = Hash.new
            @activation_ports = Hash.new
            @task_ports = Hash.new

            @widget = Vizkit.default_loader.WbcStatusGui
            @wbc_proxy = @wbc_task.to_proxy

            @wbc_proxy.port("solver_output").once_on_data do |data|
                @widget.configureSolverUI data.names
                connectSolverUI
            end

            # (Re-)Configure UI if the wbc_config property changes
            @wbc_proxy.property("wbc_config").on_change do |config|
                @widget.configureTaskUI config
                connectTaskUI config
            end

            @wbc_proxy.on_reachable do
                @widget.setEnabled true
            end

            @wbc_proxy.on_unreachable do |state|
                @widget.setEnabled false
            end

            @widget.connect(SIGNAL('reconfigureWBC()')) do
                reconfigureWBC
            end

            @widget.connect(SIGNAL('importConfig(QString, QString)')) do |file_name, section_name|
              begin
                splitted = section_name.split(",")
                @wbc_task.apply_conf_file(file_name, splitted, true)
                reconfigureTasks [@wbc_task, @solver_task]
              rescue Exception => e
                 showMessageBox Qt::MessageBox.Warning, "Import Error",
                             "Unable to apply config section(s) " + splitted.to_s + " from file " + file_name
              end
            end

            @widget.connect(SIGNAL('exportConfig(QString, QString)')) do |file_name, section_name|
              begin
                @wbc_task.save_conf(file_name, section_name)
              rescue Exception => e
                 showMessageBox Qt::MessageBox.Warning, "Import Error",
                                "Unable to save config to file " + file_name
              end
            end
        end

      def reconfigureWBC()
          joint_weights = @widget.getJointWeights
          @wbc_task = Orocos::name_service.get_provides "wbc::WbcTask"
          wbc_config = @wbc_task.wbc_config
          wbc_config.each do |cfg|
              task_widget = @widget.getTaskInterface cfg.name
              cfg.priority = task_widget.getPriority
              cfg.weights = task_widget.getTaskWeights
              cfg.activation = task_widget.getActivation
          end
          @wbc_task.wbc_config = wbc_config
          reconfigureTasks [@wbc_task]
      end

      def reconfigureTasks(tasks)
          tasks.each{ |t| t.stop if t.running?}
          tasks.each{ |t|
              t.cleanup
              t.configure
              t.start
          }
      end

      def showMessageBox(type, title, text)
          box = Qt::MessageBox.new
          box.icon = type
          box.text = text
          box.windowTitle = title
          box.show
      end

    end # class
end # module
