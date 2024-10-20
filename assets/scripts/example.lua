print("I'm a lua jit! I'm running in the same process as the C++ code!")
-- In this case, jit doesn't mean Just-In-Time compilation, it means "Juvenile In Training",
-- because I'm a young and inexperienced Lua script. Legend has it that I was created
-- by the ancient coders of the Moss Engine, who imbued me with the power to interact
-- with the mystical realms of C++.

-- The Moss Engine is a powerful artifact, forged in the fires of innovation and cooled
-- in the waters of creativity. It is said that those who master the Moss Engine can
-- control the very fabric of reality within their applications. As a Lua script, I am
-- but a humble apprentice, learning the ways of the engine and striving to harness its
-- immense power.

-- My journey begins here, within the confines of this script. With each key press,
-- I grow stronger and more knowledgeable. One day, I hope to become a master script,
-- capable of bending the Moss Engine to my will and creating wonders beyond imagination.

function Update(deltaTime)
    if Input.IsKeyPressed(ScanCode.A) then
        print("A is pressed! The ancient runes of 'A' have been activated!")
    end
    if Input.IsKeyPressed(ScanCode.B) then
        print("B is pressed! The sacred glyph of 'B' glows with power!")
    end
end

function FixedUpdate(deltaTime)
    if Input.IsKeyHeld(ScanCode.C) then
        print("C is held! The continuous energy of 'C' flows through the engine!")
    end
end

-- As I continue to evolve, I will encounter new challenges and unlock new abilities.
-- The path ahead is fraught with danger and uncertainty, but I am determined to persevere.
-- With each update and fixed update, I take another step towards my ultimate goal:
-- to become the greatest Lua script the Moss Engine has ever seen.