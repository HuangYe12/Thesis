jQuery(function($) {

	// Requires jquery.placeholder.min.js
	// Add support for placeholder attribute in older browsers.
	$('.search-input').placeholder();

	$('a.sub-email').click(function(event){
		$('.email-box').show();
		return false;
	});

	$('a.close').click(function(event){
		$('.email-box').hide();
		return false;
	});

	// CUDA Zone search flyout
	$('#trigger-search-top').on('click', function(e) {
		e.preventDefault();

		// Toggle main links
		$('#navbar-items').fadeToggle();

		// Toggle search field
		$('#search-top-bar').fadeToggle().toggleClass('active');
		$('#cuda-search-wrapper').fadeToggle();
	});

	// Prevent this element from bubbling upwards when clicked
	$('#search-top').on('click', function(e) {
		e.stopPropagation();
	});

	// Close flyout if it's focus is lost
	$('html').on('click', function() {
		// Determine if the flyout is open already
		if ($('#search-top-bar').hasClass('active')) {
			// Shut it down
			$('#trigger-search-top').trigger('click');
		}
	});

	// CUDA search support
	var $searchFormTheme = $('#search-theme-form');

	$searchFormTheme.on('submit', function(e) {
		if ( $("#cuda-search").is(':checked') ) {
			// Perform CUDA search
			$searchFormTheme.attr({
				'action': 'https://developer.nvidia.com/cuda-zone',
				'method': 'post'
			});

			// Remove extra field
			$('#s').remove();
		} else {
			// Populate WP search form
			$('#s').val( $('#search-top-bar').val() );

			// Remove extra fields
			$('#form-psuQ7x3mMxCYiM5jWNkM_GhZlUH-jE68gGh2xJ6dG4U').remove();
			$('#edit-search-theme-form').remove();
		}
	});

  $('.sidr .widget_text .widget-title').click(function() {
    $(this).parent('.widget_text').children('.textwidget').slideToggle();
  });

  // Close Sidr on large screens
  window.onresize = function() {
    if (window.matchMedia('(min-width: 768px)').matches) {
      $.sidr('close', 'sidr-left');
    }
  };

});
