<?php
function socket_read_normal($socket, $end=array("\r", "\n"))
{
    if(is_array($end)){
        foreach($end as $k=>$v){
            $end[$k]=$v{0};
        }
        $string='';
        while(TRUE){
            $char=socket_read($socket,1);
            $string.=$char;
            foreach($end as $k=>$v){
                if($char==$v){
                    return $string;
                }
            }
        }
    }else{
        $endr=str_split($end);
        $try=count($endr);
        $string='';
        while(TRUE){
            $ver=0;
            foreach($endr as $k=>$v){
                $char=socket_read($socket,1);
                $string.=$char;
                if($char==$v){
                    $ver++;
                }else{
                    break;
                }
                if($ver==$try){
                    return $string;
                }
            }
        }
    }
}
