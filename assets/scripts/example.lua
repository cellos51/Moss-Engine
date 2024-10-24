local update
local fixedUpdate

local count = 0
local maxCount = 1000

update = Callback.Update(function(deltaTime)
    local entity = Entity.new()
    entity.Transform.Position = Vector3.new(math.random(-1000, 1000) / 150, math.random(-1000, 1000) / 300, -7)
    entity.Transform.Scale = Vector3.one() * math.random(1, 5) / 100

    count = count + 1
    if count >= maxCount then
        update:Disconnect()
    end
end)

fixedUpdate = Callback.FixedUpdate(function(fixedDeltaTime)
    
end)