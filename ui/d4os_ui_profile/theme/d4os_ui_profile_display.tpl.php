<?php print $profile->text; ?>
<style>
  .profile_block {
    border:1px solid white;
    padding:10px;
    margin:10px;
    -moz-border-radius: 20px;
    -webkit-border-radius: 20px;
  }
  .tabNavigation {
    margin: 10px;
    padding: 0;
    list-style: none;
    text-align: center;
  }
  .tabNavigation li {
    display: inline;
    margin-right: 1px;
  }
  .tabNavigation li a {
    padding: 4px 10px;
    color: #fff;
    border: 1px solid #fff;
    line-height: 1em;
    text-align: center;
    text-decoration: none;
    -moz-border-radius: 10px;
    -webkit-border-radius: 10px;

  }
  .tabNavigation li a.selected {
    background: #900;
  }
  .tabNavigation li a:hover, #navigation li a:focus, #navigation li a:active {
    background: #900;
  }
</style>
<?php print $profile->messages; ?>
<div class="tabs">
  <ul class="tabNavigation">
<?php if($profile->show_inworld): ?>
    <li><a href="#profile_inworld"><?php print t('Inworld'); ?></a></li>
<?php endif; // inworld ?>
<?php if($profile->show_web): ?>
    <li><a href="#profile_web"><?php print t('Web'); ?></a></li>
<?php endif; // web ?>
<?php if($profile->show_interests): ?>
    <li><a href="#profile_interests"><?php print t('Interests'); ?></a></li>
<?php endif; // interests ?>
<?php if($profile->show_picks): ?>
    <li><a href="#profile_picks"><?php print t('Picks'); ?></a></li>
<?php endif; // picks ?>
<?php if($profile->show_classifieds): ?>
    <li><a href="#profile_classifieds"><?php print t('Classifieds'); ?></a></li>
<?php endif; // classifieds ?>
<?php if($profile->show_firstlife): ?>
    <li><a href="#profile_firstlife"><?php print t('First life'); ?></a></li>
<?php endif; // firstlife ?>
<?php if($profile->show_my_notes): ?>
    <li><a href="#profile_mynotes"><?php print t('My notes'); ?></a></li>
<?php endif; // mynotes ?>
<?php if($profile->show_options): ?>
    <li><a href="#profile_options"><?php print t('Options'); ?></a></li>
<?php endif; // options ?>
  </ul>
<?php if($profile->show_inworld): ?>
  <div id="profile_inworld">
    <h2><?php print t('Inworld'); ?></h2>
    <div class="profile_block">
      <dl>
        <dt><?php print t('Photo'); ?></dt>
          <dd><?php print $profile->grid_profileImage; ?></dd>
        <dt><?php print t('Key'); ?></dt>
          <dd><?php print $profile->grid_UUID; ?></dd>
        <dt><?php print t('Firstname'); ?></dt>
          <dd><?php print $profile->grid_username; ?></dd>
        <dt><?php print t('Lastname'); ?></dt>
          <dd><?php print $profile->grid_lastname; ?></dd>
        <dt><?php print t('Born'); ?></dt>
          <dd><?php print $profile->grid_created; ?></dd>
<?php if($profile->can_edit): ?>
        <dt><?php print t('Last connection'); ?></dt>
          <dd><?php print $profile->grid_lastLogin; ?></dd>
        <dt><?php print t('Home region'); ?></dt>
          <dd><?php print $profile->grid_homeRegion; ?></dd>
<?php endif; // can_edit ?>
        <dt><?php print t('About text'); ?></dt>
          <dd><?php print $profile->grid_profileAboutText; ?></dd>
      </dl>
    </div>
  </div>
<?php endif; // inworld ?>
<?php if($profile->show_web): ?>
  <div id="profile_web">
    <h2><?php print t('Web'); ?></h2>
    <div class="profile_block">
      <dl>
        <dt><?php print t('Web'); ?></dt>
          <dd><?php print $profile->grid_web; ?></dd>
      </dl>
      <iframe width="600px" height="400px" src="http://d4os.org"></iframe>
    </div>
  </div>
<?php endif; // web ?>
<?php if($profile->show_interests): ?>
  <div id="profile_interests">
    <h2><?php print t('Interests'); ?></h2>
    <div class="profile_block">
      <dl>
        <dt><?php print t('Interests'); ?></dt>
          <dd><?php print $profile->grid_interests; ?></dd>
      </dl>
    </div>
  </div>
<?php endif; // interests ?>
<?php if($profile->show_picks): ?>
  <div id="profile_picks">
    <h2><?php print t('Picks'); ?></h2>
    <div class="profile_block">
      <dl>
        <dt><?php print t('Picks'); ?></dt>
          <dd><?php print $profile->grid_picks; ?></dd>
      </dl>
    </div>
  </div>
<?php endif; // picks ?>
<?php if($profile->show_classifieds): ?>
  <div id="profile_classifieds">
    <h2><?php print t('Classifieds'); ?></h2>
    <div class="profile_block">
      <dl>
        <dt><?php print t('Classifieds'); ?></dt>
          <dd><?php print $profile->grid_classifieds; ?></dd>
      </dl>
    </div>
  </div>
<?php endif; // classifieds ?>
<?php if($profile->show_firstlife): ?>
  <div id="profile_firstlife">
    <h2><?php print t('First life'); ?></h2>
    <div class="profile_block">
      <dl>
        <dt><?php print t('Photo'); ?></dt>
          <dd><?php print $profile->grid_profileFirstImage; ?></dd>
        <dt><?php print t('Info'); ?></dt>
          <dd><?php print $profile->grid_profileFirstText; ?></dd>
      </dl>
    </div>
  </div>
<?php endif; // firstlife ?>
<?php if($profile->show_my_notes): ?>
  <div id="profile_mynotes">
    <h2><?php print t('My notes'); ?></h2>
    <div class="profile_block">
      <dl>
        <dt><?php print t('Notes'); ?></dt>
          <dd><?php print $profile->my_notes; ?></dd>
      </dl>
    </div>
  </div>
<?php endif; // mynotes ?>
<?php if($profile->show_options): ?>
  <div id="profile_options">
    <h2><?php print t('Options'); ?></h2>
    <div class="profile_block">
      <?php print $profile->options; ?>
    </div>
  </div>
<?php endif; // mynotes ?>
</div>
<script>
$(function () {
    var tabContainers = $('div.tabs > div');
    
    $('div.tabs ul.tabNavigation a').click(function () {
        tabContainers.hide().filter(this.hash).show();
        
        $('div.tabs ul.tabNavigation a').removeClass('selected');
        $(this).addClass('selected');
        
        return false;
    }).filter(':first').click();
});
</script>