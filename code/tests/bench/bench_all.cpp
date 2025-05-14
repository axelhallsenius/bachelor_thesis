#include "draw.h"
#include <benchmark/benchmark.h>

static void BM_geod_to_meter(benchmark::State& state) {
  //setup here
  point_geod geop;
  point_tm_grid gridp;
  geop.deg_lat = -37.84175000;
  geop.deg_long = 144.92936000;

  for (auto _ : state) {
    //this code gets timed

    gridp = geod_to_utm_grid(geop);
  }
}

//register function as a benchmark
BENCHMARK(BM_geod_to_meter);

static void BM_meter_to_geod(benchmark::State& state) {
  //setup here
  point_geod geop;
  point_geod ans;
  point_tm_grid gridp;
  ans.deg_lat = -42.1634034242;
  ans.deg_long = 146.6015625000;
  gridp.x = 467086.882;
  gridp.y = 5332004.203;


  for (auto _ : state) {
    //this code gets timed
    utm_zone zone = utm_zone_from_geod(ans);
    geop = utm_grid_to_geod(gridp,zone);
    zone = utm_zone_from_geod(ans);
    geop = utm_grid_to_geod(gridp,zone);
  }
}
//register function as a benchmark
BENCHMARK(BM_meter_to_geod);

static void BM_zone_transfer(benchmark::State& state) {
  point_geod geop;
  point_geod ans;
  point_tm_grid gridp;
  ans.deg_lat = -42.1634034242;
  ans.deg_long = 146.6015625000;
  gridp.x = 467086.882;
  gridp.y = 5332004.203;
  point_geod other_zone = {-37.37000, 120.230000};



  for (auto _ : state) {
    //this code gets timed
    utm_zone zone = utm_zone_from_geod(other_zone);
    geop = utm_grid_to_geod(gridp,zone);
    zone = utm_zone_from_geod(ans);
    geop = utm_grid_to_geod(gridp,zone);
  }
}
BENCHMARK(BM_zone_transfer);


//run benchmarks
BENCHMARK_MAIN();
