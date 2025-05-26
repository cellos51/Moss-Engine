for i = 1, 100 do
    local mesh = MeshInstance.new()
    mesh.Transform.Position = Vector3.new(math.random(-100, 100), math.random(-100, 100), math.random(-100, 100))
    
    local random = math.random(1, 2)
    if random == 1 then
        mesh.Mesh = "icosphere.glb"
    elseif random == 2 then
        mesh.Mesh = "monkey.glb"
    end
end