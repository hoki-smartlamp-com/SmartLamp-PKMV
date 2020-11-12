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
    variasi: 0,
    power: false,
    feedback: {},
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
    setVariation(state, payload) {
      state.variasi = payload;
    },
    setPower(state, payload) {
      state.power = payload;
    },
    setFeedback(state, payload) {
      state.feedback = payload;
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
          commit("setError", null);
        })
        .catch((err) => {
          commit("setLoading", false);
          commit("setError", err.message);
          // console.log(err);
        });
    },
    userSignUp({ commit }, payload) {
      commit("setLoading", true);
      db.ref(`lamp/${payload.lamp_id}`)
        .once("value")
        .then(function(res) {
          if (!res.val()) {
            // console.log(key);
            commit(
              "setError",
              "Maaf, Kode seri lampu yang anda masukkan tidak ada"
            );
            commit("setLoading", false);
          } // null
          else {
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
          }
        });
    },
    getUserData({ commit }, payload) {
      // commit("setLoading", true);
      db.ref("users")
        .child(payload)
        .on(
          "value",
          (res) => {
            commit("setUser", res.val());

            db.ref(`lamp/${res.val().lamp_id}`).on("value", (data) => {
              if (data.exists()) {
                commit("setColor", data.val().warna);
                commit("setVariation", data.val().variasi);
                commit("setPower", data.val().power);
                commit("setError", null);
              } else
                commit(
                  "setError",
                  "Oops! Kode Seri Lampu kamu tidak ditemukan, silahkan menghubungi admin untuk informasi lebih lanjut"
                );
              commit("setUID", payload);
              commit("setLoading", false);
            });

            if (router.history.current.path == "/login") {
              router.push("/");
            }
          },
          (e) => console.log(e)
        );
    },
    autoSignIn({ commit, state, dispatch }, payload) {
      if (!state.user) {
        commit("setLoading", true);
        dispatch("getUserData", payload.uid);
        // commit("setUser", { email: payload.uid });
      }
    },
    userSignOut({ commit }) {
      firebase
        .auth()
        .signOut()
        .then(() => {
          commit("setUser", null);
          commit("setVariation", null);
          commit("setPower", null);
          commit("setError", null);
          commit("setUID", null);
          router.replace("/login");
        });
    },
    setColor({ dispatch, state }, payload) {
      db.ref(`lamp/${state.user.lamp_id}`)
        .child("warna")
        .set(payload);
      dispatch("variationChanged", 0);
    },
    setModalColor({ commit }) {
      commit("setModalColor");
    },
    updateUser({ commit, state }, payload) {
      db.ref(`lamp/${payload.lamp_id}`)
        .once("value")
        .then(function(res) {
          if (res.exists()) {
            commit("updateUser", payload);
            db.ref(`users/${state.uid}`)
              .set(payload)
              .then(() => {
                router.push("/");
              });
          } else commit("setError", "Oops! Kode Seri Lampu yang anda masukkan salah");
        });
    },
    variationChanged({ commit, state }, payload) {
      db.ref(`lamp/${state.user.lamp_id}`)
        .child("variasi")
        .set(payload);
      commit("setVariation", payload);
    },
    setPower({ commit, state }, payload) {
      db.ref(`lamp/${state.user.lamp_id}`)
        .child("power")
        .set(payload);
      commit("setPower", payload);
    },
    sendFeedback({ commit, state }, payload) {
      commit("setLoading", true);
      db.ref("feedback")
        .push({
          email: state.user.email,
          username: state.user.username,
          title: payload.title,
          body: payload.body,
        })
        .then(() => {
          router.push("/");
          setTimeout(() => {
            commit("setLoading", false);
          }, 500);
        });
    },
    addNewLamp({ commit }, payload) {
      commit("setLoading", true);
      db.ref(`lamp/${payload}`)
        .once("value")
        .then(function(res) {
          if (!res.val()) {
            db.ref(`lamp/${payload}`)
              .set({
                power: true,
                variasi: 5,
                warna: {
                  red: 255,
                  green: 255,
                  blue: 255,
                  bright: 100,
                },
              })
              .then(() => {
                commit("setLoading", false);
              });
          } // null
          else {
            // console.log(key);
            commit(
              "setError",
              "Maaf, Kode seri lampu yang anda masukkan sudah ada"
            );
            commit("setLoading", false);
          }
        });
    },
    getFeedback({ commit }) {
      return new Promise((resolve) => {
        db.ref("feedback").on("value", (data) => {
          commit("setFeedback", data.val());
          resolve(data.val());
        });
      });
    },
    getLampId() {
      return new Promise((resolve) => {
        db.ref("lamp").on("value", (res) => {
          resolve(res.val());
        });
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
