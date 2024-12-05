print("Hello from Lua!")

Callback.Update:Connect(function(deltaTime)
    if Input.IsKeyPressed(ScanCode.Space) then
        print("Space key is pressed!")
    end
    if Input.IsKeyReleased(ScanCode.Space) then
        print("Space key is released!")
    end
end)

Callback.FixedUpdate:Connect(function(deltaTime)
    
end)