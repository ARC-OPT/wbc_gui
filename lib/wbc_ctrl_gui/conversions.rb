module Conversions

    def RigidBodyStateSE3ToRawPosition(cs)
        return cs.pose.position.to_a
    end

    def RigidBodyStateSE3ToRawSpeed(cs)
        return cs.twist.linear.to_a
    end

    def rawPoseToRigidBodyStateSE3(arr)
        raise "Size of input array has to be 6!" if arr.size() != 6
        cs = Types.base.samples.RigidBodyStateSE3.new
        cs.pose.position = Types.base.Vector3d.new(*arr[0..2])
        cs.pose.orientation = Types.base.Quaterniond.from_euler(Types.base.Vector3d.new(*arr[3..5].reverse), 2, 1, 0)
        cs.twist.linear = Types.base.Vector3d.new(0,0,0)
        cs.twist.angular = Types.base.Vector3d.new(0,0,0)
        cs.acceleration.linear = Types.base.Vector3d.new(0,0,0)
        cs.acceleration.angular = Types.base.Vector3d.new(0,0,0)
        return cs
    end

    def RigidBodyStateSE3ToRawPose(cs)
        return cs.pose.position.to_a + cs.pose.orientation.to_euler.to_a.reverse
    end

    def rawTwistToRigidBodyStateSE3(arr)
        raise "Size of input array has to be 6!" if arr.size() != 6
        cs = Types.base.samples.RigidBodyStateSE3.new
        cs.twist.linear  = Types.base.Vector3d.new(*arr[0..2])
        cs.twist.angular = Types.base.Vector3d.new(*arr[3..5])
        return cs
    end

    def RigidBodyStateSE3ToRawTwist(cs)
        return cs.twist.linear.to_a + cs.twist.angular.to_a
    end

    def rawToWrench(arr)
        raise "Size of input array has to be 6!" if arr.size() != 6
        wrench = Types.base.samples.Wrench.new
        wrench.force  = Types.base.Vector3d.new(*arr[0..2])
        wrench.torque = Types.base.Vector3d.new(*arr[3..5])
        return wrench
    end

    def wrenchToRaw(wrench)
        return wrench.force.to_a + wrench.torque.to_a
    end

    def jointsToRawPositions(joints)
        positions = []
        joints.each do |elem|
            positions << elem.position
        end
        return positions
    end

    def rawPositionsToJoints(positions, names)
        raise "Size of input array has to be same as size of names vector!" if positions.size() != names.size
        joints = Types.base.samples.Joints::from_positions(*positions)
        joints.names = names
        return joints
    end

    def rawEffortsToJoints(efforts, names)
        raise "Size of input array has to be same as size of names vector!" if efforts.size() != names.size
        joints = Types.base.samples.Joints::from_efforts(*efforts)
        joints.names = names
        return joints
    end

    def jointsToRawSpeeds(joints)
        speeds = []
        joints.elements.each do |elem|
            speeds << elem.speed
        end
        return speeds
    end

    def jointsToRawEfforts(joints)
        efforts = []
        joints.elements.each do |elem|
            efforts << elem.effort
        end
        return efforts
    end

    def rawSpeedsToJoints(speeds, names)
        raise "Size of input array has to be same as size of names vector!" if positions.size() != names.size
        Types.base.samples.Joints::from_speeds(*speeds)
        joints.names = names
        return joints
    end

    def rawToVectorXd(arr)
        return Types.base.VectorXd.from_a arr
    end

    def vectorXdToRaw(vect)
        return vect.to_a
    end
end
