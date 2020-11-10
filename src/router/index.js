import Vue from "vue";
import VueRouter from "vue-router";
import firebase from "firebase/app";

Vue.use(VueRouter);

const routerOptions = [
  {
    path: "/",
    name: "Home",
    component: "Home",
  },
  {
    path: "/menu",
    name: "Menu",
    component: "Menu",
  },
  {
    path: "/register",
    name: "Tambah User Baru",
    component: "Register",
    meta: {
      withoutAuth: true,
    },
  },
  {
    path: "/login",
    name: "Login",
    component: "Login",
    meta: {
      withoutAuth: true,
    },
  },
  {
    path: "/feedback",
    name: "Feedback",
    component: "Feedback",
  },
  {
    path: "/list-feedback",
    name: "List Feedback",
    component: "ListFeedback",
  },
  {
    path: "/setting",
    name: "Atur Akun",
    component: "Setting",
  },
  {
    path: "/about",
    name: "About",
    component: "About",
    meta: {
      withoutAuth: true,
    },
  },
];

const routes = routerOptions.map((route) => {
  return {
    ...route,
    component: () => import(`@/views/${route.component}.vue`),
  };
});

const router = new VueRouter({
  // mode: "history",
  // base: process.env.BASE_URL,
  routes,
});

router.beforeResolve((to, from, next) => {
  const withoutAuth = to.matched.some((record) => record.meta.withoutAuth);
  const isLoggedIn = firebase.auth().currentUser;

  if (
    (withoutAuth && !isLoggedIn) ||
    (!withoutAuth && isLoggedIn) ||
    (withoutAuth && to.path == "/about")
  ) {
    next();
  } else if (withoutAuth && isLoggedIn) {
    // to root
    next("/");
  } else {
    next("/login");
  }
  //   // console.log(from);
});

export default router;
