var status_timer = Drupal.settings.d4os_ui_stats_status.timer*1000;
$.timer(status_timer, function(timer) {
   $.get('/grid/stats/status', function(data){
      $('#grid_status').replaceWith(data);
   });
});