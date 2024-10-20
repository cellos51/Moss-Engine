print("I'm a lua jit! I'm running in the same process as the C++ code!")

function Update()
    if Input.IsKeyPressed(ScanCode.A) then
        print("A is pressed!")
    end
    if Input.IsKeyPressed(ScanCode.B) then
        print("B is pressed!")
    end
end

function FixedUpdate()
    if Input.IsKeyHeld(ScanCode.C) then
        print("C is pressed!")
    end
end