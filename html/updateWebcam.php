<?php
//retrieve last image number
$img_number = (int) file_get_contents('./img_number.txt', true);

//increment it
$img_number += 1;

//write new image number
file_put_contents('./img_number.txt', $img_number);

//take still image with PiCamera and store it in '/var/www/img' directory
shell_exec("raspistill -n -w 640 -h 480 -t 1000 -o /var/www/html/img/image".$img_number.".jpg");

//save new image name
$img_name = './img/image'.$img_number.'.jpg';

//update ajax_info with most recent image name
file_put_contents('./ajax_info.txt', $img_name);
?>