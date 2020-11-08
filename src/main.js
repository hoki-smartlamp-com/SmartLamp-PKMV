import Vue from "vue";
import App from "./App.vue";
import "./registerServiceWorker";
import router from "./router";
import store from "./store";
import axios from "axios";
import vuetify from "./plugins/vuetify";
import "./firebase";
// import { db } from "./firebase";
import firebase from "firebase/app";
import VueOffline from "vue-offline";

Vue.use(VueOffline);
Vue.mixin({
  data: () => ({
    isFilled: [(v) => !!v || "Harus diisi"],
    emailRules: [
      (v) => !!v || "E-mail harus diisi",
      (v) => /.+@.+\..+/.test(v) || "E-mail tidak valid",
    ],
    nameRules: [
      (v) => !!v || "Nama belum diisi",
      (v) => (v && v.length <= 10) || "Nama harus kurang dari 10 huruf",
      (v) => (v || "").indexOf(" ") < 0 || "Tidak boleh ada spasi",
    ],
    passwordRules: [
      (v) => !!v || "Password belum diisi",
      (v) => (v && v.length >= 6) || "Password harus lebih dari 6 huruf",
      (v) => (v || "").indexOf(" ") < 0 || "Tidak boleh ada spasi",
    ],
    cantGoBack: true,
    // activeNavBtn: "home",
  }),
  methods: {
    signOut() {
      this.$store.dispatch("userSignOut");
    },
    goBack() {
      if (!this.cantGoBack) {
        this.$router.go(-1);
      } else this.$router.push("/");
    },
    canGoBack() {
      this.$router.afterEach((to, from) => {
        // console.log(from);
        if (from.name == null) {
          this.cantGoBack = true;
        } else this.cantGoBack = false;
      });
    },
    changeTitleName() {
      if (this.$route.name != "Home") {
        document.title = "Hoki - " + this.$route.name;
      } else document.title = "Hoki SmartLamp";
    },
  },
});
Vue.prototype.$axios = axios;
Vue.config.productionTip = false;

firebase.auth().onAuthStateChanged((firebaseUser) => {
  new Vue({
    router,
    store,
    vuetify,
    render: (h) => h(App),
    mounted() {
      if (firebaseUser) {
        store.dispatch("autoSignIn", firebaseUser);
      }
    },
  }).$mount("#app");
});
