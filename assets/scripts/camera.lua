local sensitivity = 0.002
local speed = 10.0

local camera = Renderer.Camera
camera.FarClipPlane = 100000

local rotation = Vector3.new(0, 0, 0)

Callbacks.Update:Connect(function(deltaTime)

    if Input.IsMouseButtonHeld(3) then
        Cursor.Locked = true

        rotation = rotation + Vector3.new(-Input.GetMouseDelta().Y, -Input.GetMouseDelta().X, 0) * sensitivity
        print(rotation)
        camera.Transform.EulerAngles = rotation
    else
        Cursor.Locked = false
    end

    local movement = Vector3.new(0, 0, 0)
    if Input.IsKeyHeld(ScanCode.W) then
        movement = movement + Vector3.new(0, 0, 1)
    end
    if Input.IsKeyHeld(ScanCode.S) then
        movement = movement + Vector3.new(0, 0, -1)
    end
    if Input.IsKeyHeld(ScanCode.A) then
        movement = movement + Vector3.new(-1, 0, 0)
    end
    if Input.IsKeyHeld(ScanCode.D) then
        movement = movement + Vector3.new(1, 0, 0)
    end
    if Input.IsKeyHeld(ScanCode.E) then
        movement = movement + Vector3.new(0, 1, 0)
    end
    if Input.IsKeyHeld(ScanCode.Q) then
        movement = movement + Vector3.new(0, -1, 0)
    end

    movement = movement.Unit

    if Input.IsKeyHeld(ScanCode["Left Shift"]) then
        movement = movement * 2
    end

    camera.Transform.Position = camera.Transform.Position + ((camera.Transform.Forward * movement.Z) + (camera.Transform.Right * movement.X) + (camera.Transform.Up * movement.Y)) * speed * deltaTime
end)