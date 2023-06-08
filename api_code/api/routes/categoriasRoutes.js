const { Router } = require('express')
const CategoriaController = require('../controllers/CategoriaController')

const router = Router()

router.get('/categorias', CategoriaController.pegaTodosOscategorias)
router.get('/categorias/:id', CategoriaController.pegaUmCategoria)
router.post('/categorias', CategoriaController.criaNivel)
router.put('/categorias/:id', CategoriaController.atualizarNivel)
router.delete('/categorias/:id', CategoriaController.apagaNivel)

module.exports = router
