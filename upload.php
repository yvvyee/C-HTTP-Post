<?php
$uploaddir = "upload/";
$uploadfile = $uploaddir . basename( $_FILES['upfile']['name']);

if(move_uploaded_file($_FILES['upfile']['tmp_name'], $uploadfile))
{
  echo "The file has been uploaded successfully";
}
else
{
  echo "There was an error uploading the file";
}
?>