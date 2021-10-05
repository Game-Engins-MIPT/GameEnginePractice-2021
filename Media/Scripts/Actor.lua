Entity = {
    Properties = {
        Name = "Static",
        MeshName = "ogrehead.mesh Imported",
        
        Controllable = 0,
        HasPhysics = 0,
    },
    
    position = Vector3(25.0, 0.0, 0.0),
}

Entity.OnInit = function()

end

Entity.OnUpdate = function(dt)

end

Entity.GetPosition = function()
    return Entity.position;
end

