var board = new Array();//
var score =0;
var top = 240;
var hasConflicted= new Array();
$(document).ready(function(){newgame()});

function newgame(){//创建新游戏
    init();//初始化棋盘格
    newNumber();//随机生成数字 两个
    newNumber();
}

function init(){//初始化棋盘格（4*4的方格，创建二维数组）
    score=0;
    document.getElementById("score").innerHTML=score;
    for(var i=0;i<4;i++){//i表示棋盘的行
        for(var j=0;j<4;j++){//j表示棋盘的列
            
            var gridCell = $("#grid_cell_"+i+"_"+j);//通过双重遍历获取每个格子的元素
            gridCell.css("top",getPosTop(i));//通过getPostTop设置每个格子距顶端的距离
            gridCell.css("left",getPosLeft(j));//通过getPostLeft设置每个格子距左端的距离
        }
    }

    for(var i=0;i<4;i++){
        board[i] = new Array();
        hasConflicted[i] = new Array();
        for(var j=0;j<4;j++){
            board[i][j]=0;
            hasConflicted[i][j] = false;
        }
    }

    updateBoardView();//初始化数字格来显示数字，在初始化棋盘之后进行

}
function updateBoardView(){
    $('.number_cell').remove();//清除所有数字格
    for(var i=0;i<4;i++){
        for(var j=0;j<4;j++){//向棋盘格加数字格
            $('#grid_container').append("<div class='number_cell' id='number_cell_"+i+"_"+j+"'></div>");
            var numberCell = $("#number_cell_"+i+"_"+j);

            if(board[i][j] == 0){//如果棋盘格的值为0，设置数字格高宽都为0
                numberCell.css("height","0px");
                numberCell.css("width","0px");
                numberCell.css("top",getPosTop(i)+50);
                numberCell.css("left",getPosLeft(j)+50);
            }
            else {//如果棋盘值不为0，设置数字格高宽，背景色、前景色 数字值
                numberCell.css("height","100px");
                numberCell.css("width","100px");
                numberCell.css("top",getPosTop(i));
                numberCell.css("left",getPosLeft(j));
                numberCell.css("background-color",getNumberBackgroundColor(board[i][j]));
                numberCell.css("color",getNumberColor(board[i][j]));
                numberCell.text(board[i][j]);
            }
            hasConflicted[i][j] = false;
        }
    }

}
function newNumber(){//随机选一个格子生成一个数字
    if(nospace(board)){
        return false;
    }

    //随机一个位置
    var randx = parseInt(Math.floor(Math.random() * 4 ));
    var randy = parseInt(Math.floor(Math.random() * 4 ));

    //设置一个时间参数 如果50次以内还未生成一个空的位置，那么就遍历格子 选第一个空的为随机的位置
    var times=0;
    while(times<50){
        if(board[randx][randy]==0)
            break;
         randx = parseInt(Math.floor(Math.random() * 4));
         randy = parseInt(Math.floor(Math.random() * 4));
        times++;
    }
    if(times==50){
        for(var i=0;i<4;i++){
            for(var j=0;j<4;j++){
                if(board[i][j]==0){
                    randx=i;
                    randy=j;
                }
            }
        }
    }


/*     var randx = Math.floor(Math.random()*4);
    var randy = Math.floor(Math.random()*4); */
    //随机一个数字
    var randomNumber = Math.random() <0.65 ? 2 :4;
    //在之前随机的位置上显示数字
    board[randx][randy] = randomNumber;
    shownumberWithAnimation(randx,randy,randomNumber);
    return true;
    
}

$(document).ready(function(){//判断键盘响应 上下左右
    $(document).keydown(function(event){
        event.preventDefault();
        switch(event.keyCode){
            case 38://上
                if(moveUp()){
                    setTimeout("newNumber()",200);
                    setTimeout("isGameOver()",300);
                }
                break;
                
            case 40://下
                if(moveDown()){
                    setTimeout("newNumber()",200);
                    setTimeout("isGameOver()",300);
                }     
                break;        
            case 37://左
                if(moveLeft()){
                    setTimeout("newNumber()",200);
                    setTimeout("isGameOver()",300);
                }
                break;
            case 39://右
                if(moveRight()){
                    setTimeout("newNumber()",200);
                    setTimeout("isGameOver()",300);
                }
                break;
            default:
                break;
        }
    })
})

function moveLeft(){
//首先判断能否向zuo移动
    if(!canMoveLeft(board)){
        return false;
    }

    for(var i=0;i<4;i++){
        for(var j=1;j<4;j++){
            if(board[i][j] !=0){//当前要移动的位置数字不为0
                for(var k=0;k<j;k++){
                    if(board[i][k]==0 && noBlockHorizontal(i,k,j,board)){ //左侧数字为0 &&左侧有数字但不相等，进行左移位
                        showMoveAnimation(i, j, i, k);//移动动画
                        board[i][k]=board[i][j];//左移
                        board[i][j]=0;
                        continue;
                    }
                    else if(board[i][k]==board[i][j] && noBlockHorizontal(i,k,j,board)&& !hasConflicted[i][k]){//左侧有数字且相等 进行相加的操作
                        showMoveAnimation(i, j, i, k);
                        board[i][k]+=board[i][j];
                        board[i][j]=0;
                        score+=board[i][k];
                        updateScore(score);
                        hasConflicted[i][k]=true;
                    }
                }
            }
        }
    }
    setTimeout("updateBoardView()", 200);//刷新数字格 显示数字 设置动画
    return true;
}

function moveUp(){
    //首先判断能否向上移动
        if(!canMoveUp(board)){
            return false;
        }
        for(var j=0;j<4;j++){
            for(var i=1;i<4;i++){
                if(board[i][j]!=0){
                    for(var k=0;k<i;k++){
                        if(board[k][j]==0 && noBlockVertical(j,k,i,board)){
                            showMoveAnimation(i,j,k,j);
                            board[k][j]=board[i][j];
                            board[i][j]=0;
                            continue;
                        }
                        else if(board[k][j]==board[i][j] && noBlockVertical(j,k,i,board) && !hasConflicted[k][j]){
                            showMoveAnimation(i,j,k,j);
                            board[k][j]+=board[i][j];
                            board[i][j]=0;
                            score+=board[k][j];
                            updateScore(score);
                            hasConflicted[k][j]=true;
                            continue;
                        } 
                    }
                }
            }
        }
        setTimeout("updateBoardView()", 200);//刷新数字格 显示数字 设置动画
        return true;
    }

    function moveRight(){
            if(!canMoveRight(board)){
                return false;
            }
            for(var i=0;i<4;i++){
                for(var j=2;j>=0;j--){
                    if(board[i][j]!=0){
                        for(var k=3;k>j;k--){
                            if(board[i][k]==0 && noBlockHorizontal(i,j,k,board)){
                                showMoveAnimation(i,j,i,k);
                                board[i][k]=board[i][j];
                                board[i][j]=0;
                                continue;
                            }
                            else if(board[i][k]==board[i][j] && noBlockHorizontal(i,j,k,board) && !hasConflicted[i][k]){
                                showMoveAnimation(i,j,i,k);
                                board[i][k]+=board[i][j];
                                board[i][j]=0;
                                score+=board[i][k];
                                updateScore(score);
                                hasConflicted[i][k]=true;
                                continue;
                            } 
                        }
                    }
                }
            }
            setTimeout("updateBoardView()", 200);//刷新数字格 显示数字 设置动画
            return true;
        }

        function moveDown(){
                if(!canMoveDown(board)){
                    return false;
                }
                for(var j=0;j<4;j++){
                    for(var i=2;i>=0;i--){
                        if(board[i][j]!=0){
                            for(var k=3;k>i;k--){
                                if(board[k][j]==0 && noBlockVertical(j,i,k,board)){
                                    showMoveAnimation(i,j,k,j);
                                    board[k][j]=board[i][j];
                                    board[i][j]=0;
                                    continue;
                                }
                                else if(board[k][j]==board[i][j] && noBlockVertical(j,i,k,board) && !hasConflicted[k][j]){
                                    showMoveAnimation(i,j,k,j);
                                    board[k][j]+=board[i][j];
                                    board[i][j]=0;
                                    score+=board[k][j];
                                    updateScore(score);
                                    hasConflicted[k][j]=true;
                                    continue;
                                } 
                            }
                        }
                    }
                }
                setTimeout("updateBoardView()", 200);//刷新数字格 显示数字 设置动画
                return true;
            }

function isGameOver(){
    if(nospace(board)&&nomove(board)){//游戏结束条件 没有空格子&&不能移动
        gameover();
    }
}

function gameover(){
    alert("游戏结束 您的得分为"+score);
}
function updateScore(){
    document.getElementById("score").innerHTML=score;
}