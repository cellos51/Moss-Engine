print("Snake Game")

local gameOver = false
local positionScale = 1 / 6
local playerPos = Vector2.new(1, 1)
local moveDirection = Vector2.new(1, 0)
local pastPositions = {playerPos} -- The first element is the head of the snake. The length of the snake is the length of the array.
local snakeLength = 1

local apple = Entity.new()
local applePos = Vector2.new(5, 5)

local entityGrid = {}

local size = 0.1
local width = 10
local height = 10
local offset = Vector3.new(-width / 2, -height / 2, 0)
for x = 0, width do
    entityGrid[x] = {}
    for y = 0, height do
        entityGrid[x][y] = Entity.new()
        entityGrid[x][y].Transform.Position = Vector3.new(x + offset.X, y + offset.Y, 0) * positionScale
        entityGrid[x][y].Transform.Scale = Vector3.zero()
    end
end

apple.Transform.Position = Vector3.new(applePos.X + offset.X, applePos.Y + offset.Y, 0) * positionScale
apple.Transform.Scale = Vector3.one() * 0.06

local tickRate = 1 / 3
local tickTimer = 0
local function Tick()
    playerPos = playerPos + moveDirection

    playerPos.X = playerPos.X % width
    playerPos.Y = playerPos.Y % height

    if entityGrid[playerPos.X][playerPos.Y].Transform.Scale.X > 0 then
        print("Game Over")
        gameOver = true
        return
    elseif playerPos == applePos then
        snakeLength = snakeLength + 1

        local emptyPositions = {}
        for x = 0, width - 1 do
            for y = 0, height - 1 do
                if entityGrid[x][y].Transform.Scale.X == 0 then
                    table.insert(emptyPositions, Vector2.new(x, y))
                end
            end
        end

        applePos = emptyPositions[math.random(1, #emptyPositions)]

        apple.Transform.Position = Vector3.new(applePos.X + offset.X, applePos.Y + offset.Y, 0) * positionScale
    end

    entityGrid[pastPositions[1].X][pastPositions[1].Y].Transform.Scale = Vector3.zero()
    entityGrid[pastPositions[#pastPositions].X][pastPositions[#pastPositions].Y].Transform.Scale = Vector3.one() * size
    if snakeLength < #pastPositions then
        table.remove(pastPositions, 1)
    end

    entityGrid[playerPos.X][playerPos.Y].Transform.Scale = Vector3.one() * 0.12

    table.insert(pastPositions, playerPos)
end

Callback.Update:Connect(function(deltaTime)
    if Input.IsKeyPressed(ScanCode.Up) then
        moveDirection = Vector2.new(0, 1)
    end
    if Input.IsKeyPressed(ScanCode.Down) then
        moveDirection = Vector2.new(0, -1)
    end
    if Input.IsKeyPressed(ScanCode.Left) then
        moveDirection = Vector2.new(-1, 0)
    end
    if Input.IsKeyPressed(ScanCode.Right) then
        moveDirection = Vector2.new(1, 0)
    end

    tickTimer = tickTimer + deltaTime
    if tickTimer >= tickRate and gameOver == false then
        tickTimer = 0
        Tick()
    end

    if Input.IsKeyPressed(ScanCode.Space) then
        snakeLength = snakeLength + 1
    end
end)