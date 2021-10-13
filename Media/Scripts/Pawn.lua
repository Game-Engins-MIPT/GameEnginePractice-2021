Entity = {
    Properties = {
        Controllable = 1,
        HasPhysics = 0,
		IsStatic = 0,
    },
    
    Parameters = {
        move_speed = 10.0,
        rotate_speed = 5.0,
    },
    
    Camera = {
        offset = Vector3(20.0, 25.0, 45.0),
    },
    
    up_vector = Vector3(0.0, 1.0, 0.0),
    radian = Radian(0.0),
    orientation = Quaternion(Radian(0.0), Vector3(0.0, 1.0, 0.0)),
    position = Vector3(0.0, 0.0, 0.0),
    forward_vector = Vector3(0.0, 0.0, 1.0),
}

Entity.OnInit = function()
	Entity.orientation = Quaternion(Entity.radian, Entity.up_vector);
    Entity.position = Vector3(0.0, 0.0, 0.0);
end

Entity.OnUpdate = function(dt)
    local deltaMoveVelocity = 0.0;
    local deltaRotationVelocity = 0.0;
    
    if (inputHandler:isCommandActive(0)) then
        deltaRotationVelocity = deltaRotationVelocity + Entity.Parameters.rotate_speed;
    end
    if (inputHandler:isCommandActive(1)) then
        deltaRotationVelocity = deltaRotationVelocity - Entity.Parameters.rotate_speed;
    end
    if (inputHandler:isCommandActive(2)) then
        deltaMoveVelocity = deltaMoveVelocity + Entity.Parameters.move_speed;
    end
    if (inputHandler:isCommandActive(3)) then
        deltaMoveVelocity = deltaMoveVelocity - Entity.Parameters.move_speed;
    end
    
    deltaMoveVelocity = deltaMoveVelocity * dt;
    deltaRotationVelocity = deltaRotationVelocity * dt;
    
    Entity.radian = Entity.radian + Radian(deltaRotationVelocity);
    
    Entity.orientation:setOrientation(Entity.radian, Entity.up_vector);
    Entity.position = Entity.position + Entity.orientation * Entity.forward_vector * deltaMoveVelocity;
end

Entity.GetPosition = function()
    return Entity.position;
end

Entity.GetCameraPosition = function()
    local pos = Vector3(0.0, 0.0, 0.0);
    pos.x = Entity.Camera.offset.x + Entity.position.x;
    pos.y = Entity.Camera.offset.y + Entity.position.y;
    pos.z = Entity.Camera.offset.z + Entity.position.z;
    return pos;
end

Entity.GetCameraOffset = function()
    return Entity.Camera.offset;
end

Entity.SetCameraOffset = function(v)
    Entity.Camera.offset.x = v.x;
    Entity.Camera.offset.y = v.y;
    Entity.Camera.offset.z = v.z;
end

Entity.GetOrientation = function()
    return Entity.orientation;
end
