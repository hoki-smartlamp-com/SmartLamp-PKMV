import Vue from "vue";
import Vuex from "vuex";
import firebase from "firebase/app";
import { db } from "../firebase";
import "firebase/auth";
import "firebase/messaging";
import router from "@/router";
// import axios from "axios";

Vue.use(Vuex);

export default new Vuex.Store({
  state: {
    token: null,
    user: null,
    uid: null,
    error: null,
    loading: false,
    modalEditColor: false,
    color: {
      red: 255,
      green: 255,
      blue: 255,
      bright: 100,
    },
  },
  mutations: {
    setUser(state, payload) {
      state.user = payload;
    },
    setUID(state, payload) {
      state.uid = payload;
    },
    setError(state, payload) {
      state.error = payload;
    },
    setLoading(state, payload) {
      state.loading = payload;
    },
    setModalColor(state) {
      state.modalEditColor = !state.modalEditColor;
    },
    setColor(state, payload) {
      state.color = payload;
    },
    updateUser(state, payload) {
      state.user = payload;
    },
    // setUsername(state, payload) {
    //   state.user.username = payload;
    // },
    // setLampId(state, payload) {
    //   state.user.lamp_id = payload;
    // },
  },
  actions: {
    userSignIn({ commit, dispatch }, payload) {
      commit("setLoading", true);
      firebase
        .auth()
        .signInWithEmailAndPassword(payload.email, payload.password)
        .then((firebaseUser) => {
          dispatch("getUserData", firebaseUser.user.uid);
        })
        .catch((err) => {
          commit("setLoading", false);
          commit("setError", err.message);
          // console.log(err);
        });
    },
    userSignUp({ commit }, payload) {
      commit("setLoading", true);
      firebase
        .auth()
        .createUserWithEmailAndPassword(payload.email, payload.password)
        .then((firebaseUser) => {
          db.ref("users")
            .child(firebaseUser.user.uid)
            .set({
              username: payload.username,
              email: firebaseUser.user.email,
              role: "user",
              lamp_id: payload.lamp_id,
            })
            .then(() => {
              commit("setLoading", false);
              router.push("/login");
            });
          firebase.auth().signOut();
        })
        .catch((error) => {
          commit("setError", error.message);
        });
    },
    getUserData({ commit }, payload) {
      commit("setLoading", true);
      db.ref("users")
        .child(payload)
        .on(
          "value",
          (res) => {
            commit("setUser", res.val());

            db.ref(`lamp/${res.val().lamp_id}`)
              .child("warna")
              .on("value", (data) => {
                commit("setColor", data.val());
                commit("setUID", payload);
                commit("setLoading", false);
                commit("setError", null);
              });

            if (router.history.current.path == "/login") {
              router.push("/");
            }
          },
          (e) => console.log(e)
        );
    },
    autoSignIn({ state, dispatch }, payload) {
      if (!state.user) {
        dispatch("getUserData", payload.uid);
        // commit("setUser", { email: payload.uid });
      }
    },
    userSignOut({ commit }) {
      firebase
        .auth()
        .signOut()
        .then(() => router.replace("/login"));
      commit("setUser", null);
    },
    setColor({ state }, payload) {
      db.ref(`lamp/${state.user.lamp_id}`)
        .child("warna")
        .set(payload);
    },
    setModalColor({ commit }) {
      commit("setModalColor");
    },
    updateUser({ commit, state }, payload) {
      commit("updateUser", payload);
      db.ref(`users/${state.uid}`)
        .set(payload)
        .then(() => {
          router.push("/");
        });
    },
    // getData({ commit }) {
    //   db.ref("aturData").on("value", (data) => {
    //     commit("setData", data.val());
    //   });
    //   db.ref("dataHardware").on("value", (data) => {
    //     commit("setHardwareData", data.val());
    //   });
    //   db.ref("alatTerbuka").on("value", (data) => {
    //     commit("setToolData", data.val());
    //   });
    // },
    // setData(commit, payload) {
    //   db.ref("aturData")
    //     .set({
    //       berat: payload.berat,
    //       jenisKopi: payload.jenisKopi,
    //     })
    //     .then(router.push("/"));
    // },
    // setToolData({ commit }, payload) {
    //   commit("setLoading", true);
    //   db.ref("alatTerbuka")
    //     .set(payload)
    //     .then(() => commit("setLoading", false));
    // },
    // requestPermission({ dispatch }) {
    //   firebase
    //     .messaging()
    //     .requestPermission()
    //     .then(() => dispatch("handleTokenRefresh"))
    //     .catch((err) => console.log(err));
    // },
    // handleTokenRefresh() {
    //   firebase
    //     .messaging()
    //     .getToken()
    //     .then((token) => {
    //       // commit("setToken", {
    //       //   token: token,
    //       // });
    //       db.ref("token")
    //         .orderByChild("token")
    //         .equalTo(token)
    //         .once("value")
    //         .then((snapshot) => {
    //           if (!snapshot.val()) {
    //             db.ref("token").push({
    //               token: token,
    //             });
    //           }
    //         });
    //     });
    // },
  },
  modules: {},
});
