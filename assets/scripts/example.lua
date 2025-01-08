for i = 1, 1000 do
    local mesh = MeshInstance.new()
    mesh.Transform.Position = Vector3.new(math.random(-100, 100), math.random(-100, 100), math.random(-100, 100))
    mesh.Mesh = "icosphere.glb"
end