Entity = {
    Properties = {
        Name = "Static",
        MeshName = "ogrehead.mesh",
        
        Controllable = 0,
        HasPhysics = 0,
        IsStatic = 1
    },
    
    up_vector = Vector3(0.0, 1.0, 0.0),
    orientation = Quaternion(Radian(0.0), Vector3(0.0, 1.0, 0.0)),
    position = Vector3(25.0, 0.0, 0.0),
}

Entity.OnInit = function()
    Entity.orientation = Quaternion(Radian(0.0), Entity.up_vector);
end

Entity.OnUpdate = function(dt)

end

Entity.GetPosition = function()
    return Entity.position;
end

Entity.GetOrientation = function()
    return Entity.orientation;
end
