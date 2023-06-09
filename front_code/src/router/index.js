import Vue from 'vue'
import VueRouter from 'vue-router'
import Main from '../views/main.vue'
import Categoria from '../views/categoria.vue'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'Main',
    component: Main
  },
  {
    path: '/categoria',
    name: 'Categoria',
    component: Categoria
  },
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
