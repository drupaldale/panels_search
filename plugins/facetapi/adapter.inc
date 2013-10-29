<?php

/**
 * @file
 * Classes used by the Facet API module.
 */

/**
 * Facet API adapter for the Search API module.
 */
class PanelsSearchFacetapiAdapter extends SearchApiFacetapiAdapter {

  /**
   * Allows the backend to initialize its query object before adding the facet filters.
   *
   * @param mixed $query
   *   The backend's native object.
   */
  public function initActiveFilters($query) {
    $search_id = $query->getOption('search id');
    $index_id = $this->info['instance'];
    $facets = $this->getEnabledFacets();
    $this->fields = array();


    // We statically store the current search per facet so that we can correctly
    // assign it when building the facets. See the build() method in the query
    // type plugin classes.
    $active = &drupal_static('search_api_facetapi_active_facets', array());
    foreach ($facets as $facet) {
      $options = $this->getFacet($facet)->getSettings()->settings;
      // The 'default_true' option is a choice between "show on all but the
      // selected searches" (TRUE) and "show for only the selected searches".
      $default_true = isset($options['default_true']) ? $options['default_true'] : TRUE;
      // The 'facet_search_ids' option is the list of selected searches that
      // will either be excluded or for which the facet will exclusively be
      // displayed.
      $facet_search_ids = isset($options['facet_search_ids']) ? $options['facet_search_ids'] : array();

      if (array_search($search_id, $facet_search_ids) === FALSE) {
        $search_ids = variable_get('search_api_facets_search_ids', array());
        if (empty($search_ids[$index_id][$search_id])) {
          // Remember this search ID.
          $search_ids[$index_id][$search_id] = $search_id;
          variable_set('search_api_facets_search_ids', $search_ids);
        }
        if (!$default_true) {
          continue; // We are only to show facets for explicitly named search ids.
        }
      }
      elseif ($default_true) {
        continue; // The 'facet_search_ids' in the settings are to be excluded.
      }
      $active[$facet['name']] = $search_id;
      $this->fields[$facet['name']] = array(
        'field'             => $facet['field'],
        'limit'             => $options['hard_limit'],
        'operator'          => $options['operator'],
        'min_count'         => $options['facet_mincount'],
        'missing'           => $options['facet_missing'],
      );
    }
  }

  /**
   * Return the base facetapi facets (enabled via searchapi facets tab)
   * and the facets enabled via the panels_search_facets plugins.
   *
   * Overrides SearchApiFacetapiAdapter::getEnabledFacets
   */
  public function getEnabledFacets($realm_name = NULL) {
    $searcher = $this->info['name'];
    $facets = facetapi_get_enabled_facets($searcher);
    $panel_facets = panels_search_facet_registry($searcher, $this);
    $facets = array_merge($facets, $panel_facets);
    return $facets;
  }

  /**
   * Wrap parent method's output so we can programatically modify settings.
   * Specifically created to add default filters.
   */
  public function getFacetSettings(array $facet, array $realm) {
    $settings = parent::getFacetSettings($facet, $realm);
    // @todo See if we should just create our own filter instead of 
    // relying on facetapi_bonus
    // enable rewrite on all facets
    $settings->settings['filters']['rewrite_items'] = array('status'=>1, 'weight'=>45);
    return $settings;
  }
}

?>
