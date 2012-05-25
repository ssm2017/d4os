<?php
/*
 * Available vars :
 * $site_name = the site name defined in site informations
 * $site_slogan = the site slogan defined in site informations
 * $site_mission = the site mission defined in site informations
 * $site_footer = the site footer defined in site informations
 * $grid_status_bloc = the d4os_ui_monitoring grid status block
 * $grid_info_block = the d4os_ui_monitoring grid infos block
 */
// change the colors depending on the online grid status
$border_dark = $GLOBALS['grid_is_online'] ? '#060' : '#600';
$border_light = $GLOBALS['grid_is_online'] ? '#080' : '#800';
$background_color = $GLOBALS['grid_is_online'] ? '#060' : '#600';
$text_shadow = $GLOBALS['grid_is_online'] ? '#0f0' : '#f00';
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="cache-control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
      <style>
        #left, #right, #header, #content, .footer-message{
            border: 2px solid <?php print $border_dark; ?>;
        }
        #left .block .content,
        #right .block .content{
            background-color: <?php print $background_color; ?>;
        }
        h1, h2 {
            text-shadow: 1px 1px 2px <?php print $text_shadow; ?>;
        }
        h2 {
            border-bottom: 2px solid <?php print $border_light; ?>;
        }
      </style>
    <link rel="stylesheet" href="<?php print $GLOBALS['base_url']. '/'. $directory; ?>/d4os_ui_loginpage.css" type="text/css" media="screen"/>
    <title><?php print $site_name; ?></title>
  </head>
  <body>
    <div id="container">
      <div id="header">
        <div class="title"><h1><?php print $site_name; ?></h1></div>
        <div class="slogan"><?php print $site_slogan; ?></div>
      </div><!-- header -->
      <div id="wrapper">
        <div id="left">
          <?php print $grid_status_bloc; ?>
        </div><!-- left -->
        <div id="right">
          <?php print $grid_info_block; ?>
        </div><!-- right -->
        <div id="content">
          <p>main content rendering the node 1 by default:</p>
          <p>To change this content, you need to copy the files ui/extra/d4os_loginpage/d4os-ui-loginpage.tpl.php and ui/extra/d4os_loginpage/d4os-ui-loginpage.css to your main site theme folder and edit them.</p>
          <?php
          $build = node_view(node_load(1));
          print drupal_render($build['body']); ?>
        </div><!-- content -->
      </div><!-- wrapper -->
      <div id="footer">
        <div class="footer-message">Welcome</div>
      </div><!-- footer -->
    </div><!-- container -->
  </body>
</html>
