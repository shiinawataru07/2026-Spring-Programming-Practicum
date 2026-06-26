def create_piece(board, pos=[0, 0]):
    global x, y
    x, y = pos
    def f(move, step):
        global x, y
        tx = x
        ty = y
        dx, dy = move
        for _ in range(step):
            tx += dx
            ty += dy
            if tx < 0 or tx >= len(board[0]) or ty < 0 or ty >= len(board) or board[ty][tx] == 'x':
                return "Wrong move"
        x = tx
        y = ty
        return [tx, ty]
    return f
        
n, start_x, start_y = map(int, input().split())

# 读取棋盘
board = []
for _ in range(n):
    row = input().strip().split()
    board.append(row)
    
# 创建棋子
piece = create_piece(board, [start_x, start_y])

# 处理移动指令
try:
    while True:
        cmd = input().strip()
        dx, dy, step = map(int, cmd.split())
        result = piece([dx, dy], step)
        print(result)
except EOFError:
    pass  # 读取到EOF时正常退出