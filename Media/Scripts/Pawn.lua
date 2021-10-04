Entity = {
    Properties = {
        Name = "Player",
        MeshName = "ogrehead.mesh Imported",
        
        Controllable = 1,
        HasPhysics = 0,
    },
    
    Parameters = {
        move_speed = 10.0,
        rotate_speed = 5.0,
    },
    
    Camera = {
        offset = Vector3(0.0, 10.0, 35.0),
    },
    
    orientation = Vector3(0.0, 0.0, 0.0),
    position = Vector3(0.0, 0.0, 0.0),
}

Entity.OnInit = function()
	Entity.orientation = Vector3(0.0, 0.0, 0.0);
	Entity.position = Vector3(0.0, 0.0, 0.0);
end

Entity.OnUpdate = function(dt)
    local deltaVelocity = 0.0;
    if (inputHandler:isCommandActive(0)) then
        deltaVelocity = deltaVelocity - Entity.Parameters.move_speed;
    end
    if (inputHandler:isCommandActive(1)) then
        deltaVelocity = deltaVelocity + Entity.Parameters.move_speed;
    end
    Entity.position.x = Entity.position.x + deltaVelocity * dt;
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
