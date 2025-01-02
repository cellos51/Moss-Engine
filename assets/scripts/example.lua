print("Parent test")

local parent = Entity.new()
local child = Entity.new()

parent.Transform.LocalScale = Vector3.one() * 0.25

child.Transform.Position = Vector3.new(0, -2, 0)
child.Transform.Parent = parent.Transform

local time = 0

Callback.Update:Connect(function(deltaTime)
    time = time + deltaTime
    parent.Transform.Position = Vector3.new(math.sin(time), 0, 0)
    parent.Transform.LocalEulerAngles = parent.Transform.LocalEulerAngles + Vector3.new(0, 0, 2) * deltaTime
end)