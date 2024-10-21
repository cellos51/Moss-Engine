print("I'm a lua jit! I'm running in the same process as the C++ code!")

local player = Entity.Create()

player.Transform.Position.Z = -6.0
player.Transform.Position.X = -2.0

player.Transform.Scale = Vector3.new(0.5, 0.5, 0.5)

local velocity = Vector3.new(0.0, 0.0, 0.0)
local gravity = Vector3.new(0.0, -9.8, 0.0)

function Update(deltaTime)
    if Input.IsKeyPressed(ScanCode.Space) then
        velocity.Y = 5.0
    end

    velocity = velocity + gravity * deltaTime
    player.Transform.Position = player.Transform.Position + velocity * deltaTime

    -- clamp positions to be within the bounds of the screen
    player.Transform.Position.Y = math.min(math.max(player.Transform.Position.Y, -3.0), 3.0)
    player.Transform.Position.X = math.min(math.max(player.Transform.Position.X, -8.0), 8.0)
end

function FixedUpdate(deltaTime)

end