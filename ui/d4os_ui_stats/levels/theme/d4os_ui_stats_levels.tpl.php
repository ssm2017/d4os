<div class="item-list">
  <ul>
    <li class="first">
      <div class="form-item" id="loginuri-wrapper">
        <label for="loginuri"><?php print t('Login uri'); ?></label>
        <div id="loginuri"><?php print $data->loginuri; ?></div>
      </div>
    </li>
    <li>
      <div class="form-item" id="par-version-wrapper">
        <label for="par-version"><?php print t('Version'); ?></label>
        <div id="par-version"><?php print $data->Version; ?></div>
      </div>
    </li>
    <li>
      <div class="form-item" id="par-uptime-wrapper">
        <label for="par-uptime"><?php print t('Uptime'); ?></label>
        <div id="par-uptime"><?php print $data->Uptime; ?></div>
      </div>
    </li>
    <li>
      <div class="form-item" id="par-ragent-wrapper">
        <label for="par-ragent"><?php print t('Root agent'); ?></label>
        <div id="par-ragent"><?php print $data->RootAg; ?></div>
      </div>
    </li>
    <li class="last">
      <div class="form-item" id="par-cagent-wrapper">
        <label for="par-cagent"><?php print t('Child agent'); ?></label>
        <div id="par-cagent"><?php print $data->ChldAg; ?></div>
      </div>
    </li>
  </ul>
  <?php print $widget; ?>
</div>