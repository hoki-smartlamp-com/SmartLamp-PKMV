<template>
  <v-app class="red">
    <offline v-if="!isOnline" />
    <template v-else>
      <v-app-bar hide-on-scroll app elevation="0" color="#121212">
        <!-- <v-app-bar-nav-icon @click.stop="drawer = !drawer"></v-app-bar-nav-icon> -->

        <v-btn icon large v-if="showBackButton && showAppBar" @click="goBack()">
          <v-icon v-if="cantGoBack">mdi-home</v-icon>
          <v-icon v-else>mdi-chevron-left</v-icon>
        </v-btn>

        <v-toolbar-title
          v-if="showAppBar"
          :class="
            $route.path == '/' || $route.path == '/menu' ? 'text-uppercase' : ''
          "
          >{{ routeName }}</v-toolbar-title
        >
        <v-spacer />
        <!-- 
        <v-btn icon to="/about" 
          <v-icon></v-icon>
        </v-btn> -->

        <v-btn icon to="/about" v-if="!showAppBar">
          <v-icon large>mdi-information-outline</v-icon>
        </v-btn>

        <v-btn
          icon
          @click="$store.dispatch('setModalColor')"
          v-else-if="$route.path == '/'"
        >
          <svg
            xmlns="http://www.w3.org/2000/svg"
            fill="white"
            width="24"
            height="24"
            viewBox="0 0 24 24"
          >
            <path
              d="M14.078 7.061l2.861 2.862-10.799 10.798-3.584.723.724-3.585 10.798-10.798zm0-2.829l-12.64 12.64-1.438 7.128 7.127-1.438 12.642-12.64-5.691-5.69zm7.105 4.277l2.817-2.82-5.691-5.689-2.816 2.817 5.69 5.692z"
            />
          </svg>
        </v-btn>
      </v-app-bar>

      <v-main style="padding: 56px 0px;">
        <Loading v-if="loading && $route.path != '/'" />
        <router-view />
        <v-snackbar
          v-model="snackbar"
          v-if="
            $route.path != '/login' &&
              $route.path != '/register' &&
              $route.path != '/setting'
          "
        >
          {{ snackbarMessage }}
          <template v-slot:action="{ attrs }">
            <v-btn color="#EA6A00" text v-bind="attrs" @click="snackbar = false"
              >Close</v-btn
            >
          </template>
        </v-snackbar>
      </v-main>

      <v-bottom-navigation
        app
        grow
        elevation="24"
        color="#3076BD"
        v-if="showBottomBar"
      >
        <v-btn to="/">
          <span>Utama</span>
          <v-icon>mdi-home</v-icon>
        </v-btn>

        <v-btn to="menu">
          <span>Menu</span>
          <v-icon>mdi-view-grid</v-icon>
        </v-btn>
      </v-bottom-navigation>
    </template>
  </v-app>
</template>

<script>
import Offline from "@/components/Offline";
import Loading from "@/components/Loading";

export default {
  name: "App",

  components: {
    Offline,
    Loading
  },

  data: () => ({
    dark: true,
    showAppBar: false,
    showBackButton: false,
    showBottomBar: false,
    routeName: "",
    snackbar: false,
    snackbarMessage: "",
    param1: true,
    param2: true
  }),

  watch: {
    $route() {
      this.showBar();
      this.canGoBack();
      this.changeTitleName();
    },
    error(val) {
      if (val) {
        this.snackbarMessage = val;
        this.snackbar = true;
      }
    }
  },

  created() {
    this.changeTitleName();
    this.canGoBack();
    this.showBar();
  },

  computed: {
    loading() {
      return this.$store.state.loading;
    },
    error() {
      return this.$store.state.error;
    }
  },

  methods: {
    showBar() {
      let route = this.$route;

      // app bar
      if (
        // route.path != "/" &&
        // route.path != "/overview" &&
        route.path != "/login" &&
        route.path != "/register"
      ) {
        this.showAppBar = true;
      } else this.showAppBar = false;

      // bottom bar
      if (route.path == "/" || route.path == "/menu") {
        this.showBottomBar = true;
      } else this.showBottomBar = false;

      // back button
      if (route.path != "/" && route.path != "/menu") {
        this.routeName = route.name;
        this.showBackButton = true;
        this.dark = true;
        // console.log(route);
      }
      // default
      else {
        this.routeName = "Hoki SmartLamp";
        this.showBackButton = false;
        this.dark = false;
      }
      // console.log(route.name);
    }
  }
};
</script>

<style>
:root {
  --bg: #121212;
  --disabled: #4d4d4d;
}
body {
  overflow-x: hidden;
  font-weight: 300;
  font-family: Verdana, Geneva, Tahoma, sans-serif;
}
.theme--light.v-application {
  background: #f5f5f5 !important;
}
.v-item-group.v-bottom-navigation--grow .v-btn {
  height: 55px !important;
}
</style>