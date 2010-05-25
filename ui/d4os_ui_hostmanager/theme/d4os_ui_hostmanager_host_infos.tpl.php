<div id="hostmanager-host">
  <div id="hostmanager-host-infos">
    <dl>
      <dt><?php print t('Expire'); ?></dt>
      <dd><?php print $host->expire; ?></dd>
      <dt><?php print t('Regions qty'); ?></dt>
      <dd><?php print count($host->region); ?></dd>
    </dl>
  </div><!-- hostmanager-host-infos -->
  <?php foreach ($host->region as $region): ?>
  <div class="hostmanager-region">
    <fieldset class="collapsible">
      <legend class="collapse-processed"><a href="#"><?php print t('Region'); ?></a></legend>
      <div class="fieldset-wrapper">
        <div class="hostmanager-region-info">
          <fieldset class="collapsible">
            <legend class="collapse-processed"><a href="#"><?php print t('Info'); ?></a></legend>
            <div class="fieldset-wrapper">
              <dl>
                <dt><?php print t('Category'); ?></dt>
                <dd><?php print $region['category']; ?></dd>
                <dt><?php print t('Entities'); ?></dt>
                <dd><?php print $region['entities']; ?></dd>
                <dt><?php print t('Uuid'); ?></dt>
                <dd><?php print $region->info->uuid; ?></dd>
                <dt><?php print t('Url'); ?></dt>
                <dd><?php print $region->info->url; ?></dd>
                <dt><?php print t('Name'); ?></dt>
                <dd><?php print $region->info->name; ?></dd>
                <dt><?php print t('Handle'); ?></dt>
                <dd><?php print $region->info->handle; ?></dd>
                <dt><?php print t('Parcel qty'); ?></dt>
                <dd><?php print count($region->data->parceldata->parcel); ?></dd>
              </dl>
            </div><!-- fieldset-wrapper -->
          </fieldset>
        </div><!-- hostmanager-region-info -->
        <div class="hostmanager-region-estate">
          <div class="hostmanager-region-estate-user">
            <fieldset class="collapsible">
              <legend class="collapse-processed"><a href="#"><?php print t('Estate user'); ?></a></legend>
              <div class="fieldset-wrapper">
                <dl>
                  <dt><?php print t('Name'); ?></dt>
                  <dd><?php print $region->data->estate->user->name; ?></dd>
                  <dt><?php print t('Uuid'); ?></dt>
                  <dd><?php print $region->data->estate->user->uuid; ?></dd>
                </dl>
              </div><!-- fieldset-wrapper -->
            </fieldset>
          </div><!-- hostmanager-region-estate-user -->
          <div class="hostmanager-region-estate-info">
            <fieldset class="collapsible">
              <legend class="collapse-processed"><a href="#"><?php print t('Estate info'); ?></a></legend>
              <div class="fieldset-wrapper">
                <dl>
                  <dt><?php print t('Name'); ?></dt>
                  <dd><?php print $region->data->estate->name; ?></dd>
                  <dt><?php print t('ID'); ?></dt>
                  <dd><?php print $region->data->estate->id; ?></dd>
                  <dt><?php print t('Parent ID'); ?></dt>
                  <dd><?php print $region->data->estate->parentid; ?></dd>
                </dl>
              </div><!-- fieldset-wrapper -->
            </fieldset>
          </div><!-- hostmanager-region-estate-info -->
          <div class="hostmanager-region-estate-flags">
            <fieldset class="collapsible">
              <legend class="collapse-processed"><a href="#"><?php print t('Estate flags'); ?></a></legend>
              <div class="fieldset-wrapper">
                <dl>
                  <dt><?php print t('Teleport'); ?></dt>
                  <dd><?php print $region->data->estate->flags['teleport']; ?></dd>
                  <dt><?php print t('Public'); ?></dt>
                  <dd><?php print $region->data->estate->flags['public']; ?></dd>
                </dl>
              </div><!-- fieldset-wrapper -->
            </fieldset>
          </div><!-- hostmanager-region-estate-flags -->
          <?php foreach($region->data->parceldata->parcel as $parcel): ?>
            <div class="hostmanager-region-estate-parcel">
              <fieldset class="collapsible">
                <legend class="collapse-processed"><a href="#"><?php print t('Parcel'); ?></a></legend>
                <div class="fieldset-wrapper">
                  <dl>
                    <dt><?php print t('Name'); ?></dt>
                    <dd><?php print $parcel->name; ?></dd>
                  </dl>
                </div><!-- fieldset-wrapper -->
              </fieldset>
            </div><!-- hostmanager-region-estate-flags -->
          <?php endforeach; // parcels ?>
        </div><!-- hostmanager-region-estate -->
      </div><!-- fieldset-wrapper -->
    </fieldset>
  </div><!-- hostmanager-region -->
  <?php endforeach; // regions ?>
</div><!-- hostmanager-host -->
<pre>
  <?php print_r(get_defined_vars()); ?>
</pre>