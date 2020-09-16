function nospace(board){//判断当前格子是否为空 非空返回true 空返回false
    for(var i=0;i<4;i++){
        for(var j=0;j<4;j++){
            if(board[i][j]==0){
                return false;
            
            }
        }
    }
    return true;
}



//为数字设置背景颜色
function getNumberBackgroundColor(number) {
    switch (number) {
        case 2:
            return "#eee4da";
            break;
        case 4:
            return "#ede0c8";
            break;
        case 8:
            return "#f2b179";
            break;
        case 16:
            return "#f59563";
            break;
        case 32:
            return "#f67c5f";
            break;
        case 64:
            return "#f65e3b";
            break;
        case 128:
            return "#edcf72";
            break;
        case 256:
            return "#edcc61";
            break;
        case 512:
            return "#9c0";
            break;
        case 1024:
            return "#33b5e5";
            break;
        case 2048:
            return "#09c";
            break;
        case 4096:
            return "#a6c";
            break;
        case 3192:
            return "#93c";
            break;
    }
    return "black";
}

function getNumberColor(number) {//设置数字颜色 2 4为灰色 其他都为白色
    if ( number <= 4 ){
        return "#776e65";
    }else{
    return "white";}
}

function getPosTop(i){
    return  top=20+i*(100+20);
}

function getPosLeft(j){
    return  left=20+j*(100+20);
}

function shownumberWithAnimation(i,j,randomNumber){//显示数字动画
    var numberCell = $("#number_cell_"+i+"_"+j);
    numberCell.css("background-color",getNumberBackgroundColor(randomNumber));
    numberCell.css("color",getNumberColor(randomNumber));
    numberCell.text(randomNumber);
    numberCell.animate({
        width: "100px",
        height: "100px",
        top: getPosTop(i),
        left: getPosLeft(j)
    }, 50);
}

function showMoveAnimation(i, j, ii, jj){//移动数字的动画 数字从(i,j)=>(ii,jj)
    var numberCell = $("#number_cell_"+i+"_"+j);
    numberCell.animate({
        top:getPosTop(ii),
        left:getPosLeft(jj)
    },200)
}
function canMoveLeft(board){
    for(var i=0;i<4;i++){
        for(var j=1;j<4;j++){
            if(board[i][j] !=0){
                if(board[i][j-1]==0 || board[i][j-1]==board[i][j]){
                    return true;
                }
            }
        }
    }
    return false;
}

function canMoveUp(board){
    for(var j=0;j<4;j++){
        for(var i=1;i<4;i++){
            if(board[i][j] !=0){
                if(board[i-1][j]==0 || board[i-1][j]==board[i][j]){
                    return true;
                }
            }
        }
    }
    return false;
}
function canMoveRight(board){
    for(var i=0;i<4;i++){
        for(var j=0;j<3;j++){
            if(board[i][j] !=0){
                if(board[i][j+1]==0 || board[i][j+1]==board[i][j]){
                    return true;
                }
            }
        }
    }
    return false;
}

function canMoveDown(board){
    for(var j=0;j<4;j++){
        for(var i=0;i<3;i++){
            if(board[i][j] !=0){
                if(board[i+1][j]==0 || board[i+1][j]==board[i][j]){
                    return true;
                }
            }
        }
        
    }
    return false;
}

function noBlockHorizontal(row,col1,col2,board){//判断水平方向是否可移动，就是判断两个格子中间是否有数字
    for(var i=col1+1;i<col2;i++){
        if(board[row][i]!=0){
            return false;
        }
    }
    return true;
}

function noBlockVertical(col,row1,row2,board){//判断垂直方向是否可移动，就是判断两个格子中间是否有数字
    for(var i=row1+1;i<row2;i++){
        if(board[i][col]!=0){
            return false;
        }
    }
    return true;
}
function nomove(board){
    if(canMoveDown(board) ||
        canMoveLeft(board) ||
        canMoveRight(board) ||
        canMoveUp(board))
        return false

        return true
}